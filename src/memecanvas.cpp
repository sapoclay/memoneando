#include "memecanvas.h"
#include "layeritem.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QWheelEvent>
#include <QScrollBar>
#include <QPen>
#include <QBrush>
#include <QFileInfo>
#include <QImageReader>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QSettings>
#include <QColorSpace>
#include <QKeyEvent>
#include <QApplication>

MemeCanvas::MemeCanvas(QWidget *parent)
    : QGraphicsView(parent)
    , m_scene(nullptr)
    , m_canvasRect(nullptr)
    , m_canvasSize(800, 600)
    , m_zoomFactor(1.0)
    , m_backgroundItem(nullptr)
    , m_dragging(false)
    , m_canvasBackgroundColor(Qt::white)
{
    setupScene();
    
    // Configurar la vista
    setDragMode(QGraphicsView::RubberBandDrag);
    setRenderHint(QPainter::Antialiasing);
    setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing, false);
    setOptimizationFlag(QGraphicsView::DontSavePainterState, true);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    
    // Permitir que el canvas reciba eventos de teclado
    setFocusPolicy(Qt::StrongFocus);
    
    // Configurar colores
    setStyleSheet("QGraphicsView { background-color: #2b2b2b; }");
    
    // Centrar el canvas
    centerCanvas();
}

MemeCanvas::~MemeCanvas()
{
    qDeleteAll(m_layers);
}

void MemeCanvas::setupScene()
{
    m_scene = new QGraphicsScene(this);
    setScene(m_scene);
    
    // Crear rectángulo del canvas
    updateCanvasRect();
    
    // Conectar señales
    connect(m_scene, &QGraphicsScene::selectionChanged, this, &MemeCanvas::onSelectionChanged);
}

void MemeCanvas::updateCanvasRect()
{
    if (m_canvasRect) {
        m_scene->removeItem(m_canvasRect);
        delete m_canvasRect;
    }
    
    // Crear nuevo rectángulo del canvas
    m_canvasRect = new QGraphicsRectItem(0, 0, m_canvasSize.width(), m_canvasSize.height());
    m_canvasRect->setPen(QPen(Qt::darkGray, 1));
    m_canvasRect->setBrush(QBrush(m_canvasBackgroundColor));
    m_canvasRect->setZValue(-1000); // Asegurar que esté al fondo
    m_scene->addItem(m_canvasRect);
    
    // Actualizar el rectángulo de la escena
    m_scene->setSceneRect(-100, -100, 
                         m_canvasSize.width() + 200, 
                         m_canvasSize.height() + 200);
}

void MemeCanvas::setCanvasSize(const QSize &size)
{
    m_canvasSize = size;
    updateCanvasRect();
    
    // Reposicionar imagen de fondo si existe
    if (m_backgroundItem) {
        QPixmap scaledPixmap = m_backgroundImage.scaled(m_canvasSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        m_backgroundItem->setPixmap(scaledPixmap);
        m_backgroundItem->setPos(0, 0);
    }
    
    centerCanvas();
    emit layersChanged();
}

QSize MemeCanvas::canvasSize() const
{
    return m_canvasSize;
}

void MemeCanvas::setZoom(qreal factor)
{
    if (factor < 0.1 || factor > 5.0) return;
    
    qreal scaleFactor = factor / m_zoomFactor;
    scale(scaleFactor, scaleFactor);
    m_zoomFactor = factor;
}

qreal MemeCanvas::zoom() const
{
    return m_zoomFactor;
}

void MemeCanvas::setCanvasBackgroundColor(const QColor &color)
{
    m_canvasBackgroundColor = color;
    if (m_canvasRect) {
        m_canvasRect->setBrush(QBrush(color));
    }
}

QColor MemeCanvas::canvasBackgroundColor() const
{
    return m_canvasBackgroundColor;
}

void MemeCanvas::centerCanvas()
{
    if (m_canvasRect) {
        fitInView(m_canvasRect, Qt::KeepAspectRatio);
        m_zoomFactor = transform().m11(); // Obtener el factor de escala actual
    }
}

void MemeCanvas::addTextLayer(const QString &text)
{
    QPointF center(m_canvasSize.width() / 2.0, m_canvasSize.height() / 2.0);
    LayerItem *textLayer = createTextLayer(text, center);
    if (textLayer) {
        // Insertar al principio para que aparezca al frente
        m_layers.prepend(textLayer);
        updateZOrder();
        selectLayer(textLayer);
        emit layersChanged();
    }
}

void MemeCanvas::addImageLayer(const QString &imagePath)
{
    QPixmap pixmap = loadImageSafely(imagePath);
    if (pixmap.isNull()) {
        qWarning() << "No se pudo cargar la imagen:" << imagePath;
        return;
    }
    
    // Escalar la imagen si es muy grande
    if (pixmap.width() > m_canvasSize.width() || pixmap.height() > m_canvasSize.height()) {
        pixmap = pixmap.scaled(m_canvasSize * 0.5, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    
    QPointF center(m_canvasSize.width() / 2.0, m_canvasSize.height() / 2.0);
    LayerItem *imageLayer = createImageLayer(pixmap, center);
    if (imageLayer) {
        // Insertar al principio para que aparezca al frente
        m_layers.prepend(imageLayer);
        updateZOrder();
        selectLayer(imageLayer);
        emit layersChanged();
    }
}

LayerItem* MemeCanvas::createTextLayer(const QString &text, const QPointF &pos)
{
    LayerItem *layer = new LayerItem(LayerItem::Text);
    layer->setText(text);
    layer->setPos(pos - layer->boundingRect().center());
    
    // Asignar nombre descriptivo
    int textLayerCount = 0;
    for (LayerItem* existingLayer : m_layers) {
        if (existingLayer->layerType() == LayerItem::Text) {
            textLayerCount++;
        }
    }
    layer->setLayerName(QString("Texto %1").arg(textLayerCount + 1));
    
    m_scene->addItem(layer);
    
    // Conectar señales
    connect(layer, &LayerItem::itemChanged, this, &MemeCanvas::layersChanged);
    
    return layer;
}

LayerItem* MemeCanvas::createImageLayer(const QPixmap &pixmap, const QPointF &pos)
{
    LayerItem *layer = new LayerItem(LayerItem::Image);
    layer->setPixmap(pixmap);
    layer->setPos(pos - layer->boundingRect().center());
    
    // Asignar nombre descriptivo
    int imageLayerCount = 0;
    for (LayerItem* existingLayer : m_layers) {
        if (existingLayer->layerType() == LayerItem::Image) {
            imageLayerCount++;
        }
    }
    layer->setLayerName(QString("Imagen %1").arg(imageLayerCount + 1));
    
    m_scene->addItem(layer);
    
    // Conectar señales
    connect(layer, &LayerItem::itemChanged, this, &MemeCanvas::layersChanged);
    
    return layer;
}

void MemeCanvas::deleteSelectedLayer()
{
    if (!m_selectedLayers.isEmpty()) {
        // Permitir eliminar todas las capas si el usuario así lo desea
        for (LayerItem* layer : m_selectedLayers) {
            m_layers.removeOne(layer);
            m_scene->removeItem(layer);
            delete layer;
        }
        m_selectedLayers.clear();
        emit layersChanged();
    }
}

void MemeCanvas::clearCanvas()
{
    // Limpiar todas las capas
    qDeleteAll(m_layers);
    m_layers.clear();
    m_selectedLayers.clear();
    
    // Limpiar imagen de fondo
    if (m_backgroundItem) {
        m_scene->removeItem(m_backgroundItem);
        delete m_backgroundItem;
        m_backgroundItem = nullptr;
    }
    m_backgroundImage = QPixmap();
    
    emit layersChanged();
}

void MemeCanvas::loadBackgroundImage(const QString &imagePath)
{
    QPixmap pixmap = loadImageSafely(imagePath);
    if (pixmap.isNull()) {
        qWarning() << "No se pudo cargar la imagen de fondo:" << imagePath;
        return;
    }
    
    m_backgroundImage = pixmap;
    
    // Remover imagen de fondo anterior si existe
    if (m_backgroundItem) {
        m_scene->removeItem(m_backgroundItem);
        delete m_backgroundItem;
    }
    
    // Escalar la imagen al tamaño del canvas
    QPixmap scaledPixmap = pixmap.scaled(m_canvasSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    
    m_backgroundItem = new QGraphicsPixmapItem(scaledPixmap);
    m_backgroundItem->setPos(0, 0);
    m_backgroundItem->setZValue(-500); // Detrás de las capas pero delante del fondo blanco
    m_scene->addItem(m_backgroundItem);
    
    emit layersChanged();
}

bool MemeCanvas::exportImage(const QString &fileName)
{
    if (!m_canvasRect) return false;
    
    // Crear una imagen del tamaño del canvas con formato estable
    QImage image(m_canvasSize, QImage::Format_ARGB32_Premultiplied);
    image.fill(m_canvasBackgroundColor);
    
    // Limpiar cualquier perfil de color problemático
    image.setColorSpace(QColorSpace());
    
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    
    // Renderizar solo el área del canvas
    m_scene->render(&painter, QRectF(), QRectF(0, 0, m_canvasSize.width(), m_canvasSize.height()));
    
    // Añadir marca de agua si está habilitada
    addWatermarkToImage(painter);
    
    painter.end();
    
    // Configurar calidad según el formato
    int quality = -1; // Calidad por defecto
    if (fileName.toLower().endsWith(".jpg") || fileName.toLower().endsWith(".jpeg")) {
        quality = 95; // Alta calidad para JPEG
    }
    
    bool success = image.save(fileName, nullptr, quality);
    
    if (!success) {
        qDebug() << "Error al guardar imagen:" << fileName;
    }
    
    return success;
}

QStringList MemeCanvas::getLayers() const
{
    QStringList layerNames;
    for (int i = 0; i < m_layers.size(); ++i) {
        LayerItem *layer = m_layers.at(i);
        QString name = layer->layerName();
        if (name.isEmpty()) {
            name = QString("Capa %1").arg(i + 1);
            if (layer->layerType() == LayerItem::Text) {
                name += " (Texto)";
            } else if (layer->layerType() == LayerItem::Image) {
                name += " (Imagen)";
            }
        }
        layerNames.append(name);
    }
    return layerNames;
}

LayerItem* MemeCanvas::getSelectedLayer() const
{
    return m_selectedLayers.isEmpty() ? nullptr : m_selectedLayers.first();
}

QList<LayerItem*> MemeCanvas::getSelectedLayers() const
{
    return m_selectedLayers;
}

void MemeCanvas::setSelectedLayers(const QList<LayerItem*> &layers)
{
    // Limpiar selección actual
    clearSelection();
    
    // Seleccionar nuevas capas
    for (LayerItem* layer : layers) {
        if (m_layers.contains(layer)) {
            m_selectedLayers.append(layer);
            layer->setSelected(true);
        }
    }
    
    // Emitir señal con la primera capa (compatibilidad)
    emit layerSelected(getSelectedLayer());
}

void MemeCanvas::addToSelection(LayerItem* layer)
{
    if (layer && m_layers.contains(layer) && !m_selectedLayers.contains(layer)) {
        m_selectedLayers.append(layer);
        layer->setSelected(true);
        emit layerSelected(getSelectedLayer());
    }
}

void MemeCanvas::removeFromSelection(LayerItem* layer)
{
    if (layer && m_selectedLayers.contains(layer)) {
        m_selectedLayers.removeOne(layer);
        layer->setSelected(false);
        emit layerSelected(getSelectedLayer());
    }
}

void MemeCanvas::clearSelection()
{
    for (LayerItem* layer : m_selectedLayers) {
        layer->setSelected(false);
    }
    m_selectedLayers.clear();
    emit layerSelected(nullptr);
}

void MemeCanvas::reorderLayers(const QStringList &newOrder)
{
    if (newOrder.size() != m_layers.size()) return;
    
    QList<LayerItem*> newLayerOrder;
    QStringList currentNames = getLayers();
    
    // Reordenar según el nuevo orden
    for (const QString &name : newOrder) {
        int oldIndex = currentNames.indexOf(name);
        if (oldIndex >= 0 && oldIndex < m_layers.size()) {
            newLayerOrder.append(m_layers.at(oldIndex));
        }
    }
    
    if (newLayerOrder.size() == m_layers.size()) {
        m_layers = newLayerOrder;
        updateZOrder();
        emit layersChanged();
    }
}

void MemeCanvas::updateZOrder()
{
    // Actualizar Z-order: primera capa en la lista = mayor Z (al frente)
    for (int i = 0; i < m_layers.size(); ++i) {
        m_layers.at(i)->setZValue(m_layers.size() - i);
    }
}

LayerItem* MemeCanvas::getLayerAt(int index) const
{
    if (index >= 0 && index < m_layers.size()) {
        return m_layers.at(index);
    }
    return nullptr;
}

void MemeCanvas::selectLayer(LayerItem *layer)
{
    if (!layer) {
        clearSelection();
        return;
    }
    
    setSelectedLayers({layer});
}

void MemeCanvas::wheelEvent(QWheelEvent *event)
{
    // Zoom con la rueda del ratón
    if (event->modifiers() & Qt::ControlModifier) {
        const qreal scaleFactor = 1.15;
        if (event->angleDelta().y() > 0) {
            // Zoom in
            if (m_zoomFactor < 5.0) {
                scale(scaleFactor, scaleFactor);
                m_zoomFactor *= scaleFactor;
            }
        } else {
            // Zoom out
            if (m_zoomFactor > 0.1) {
                scale(1.0 / scaleFactor, 1.0 / scaleFactor);
                m_zoomFactor /= scaleFactor;
            }
        }
        event->accept();
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void MemeCanvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton) {
        m_dragging = true;
        m_lastPanPoint = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    
    QGraphicsView::mousePressEvent(event);
}

void MemeCanvas::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging) {
        QPoint delta = event->pos() - m_lastPanPoint;
        m_lastPanPoint = event->pos();
        
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        event->accept();
        return;
    }
    
    QGraphicsView::mouseMoveEvent(event);
}

void MemeCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton && m_dragging) {
        m_dragging = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    
    QGraphicsView::mouseReleaseEvent(event);
}

void MemeCanvas::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    // Mantener el canvas centrado al redimensionar
    if (m_canvasRect && m_layers.isEmpty()) {
        centerCanvas();
    }
}

void MemeCanvas::onSelectionChanged()
{
    auto selectedItems = m_scene->selectedItems();
    QList<LayerItem*> selectedLayers;
    
    // Convertir elementos seleccionados a LayerItems
    for (auto item : selectedItems) {
        LayerItem *layer = qgraphicsitem_cast<LayerItem*>(item);
        if (layer && m_layers.contains(layer)) {
            selectedLayers.append(layer);
        }
    }
    
    // Actualizar nuestra lista de capas seleccionadas
    setSelectedLayers(selectedLayers);
    
    // Emitir señales para compatibilidad y selección múltiple
    emit layersSelectionChanged(selectedLayers);
    
    // Mantener compatibilidad con señal anterior
    if (selectedLayers.size() == 1) {
        emit layerSelected(selectedLayers.first());
    } else if (selectedLayers.isEmpty()) {
        emit layerSelected(nullptr);
    } else {
        // Para múltiples capas, emitir la primera como seleccionada principal
        emit layerSelected(selectedLayers.first());
    }
}

void MemeCanvas::contextMenuEvent(QContextMenuEvent *event)
{
    createContextMenu(event->pos());
}

void MemeCanvas::createContextMenu(const QPoint &position)
{
    QMenu contextMenu(this);
    
    // Icono y título del menú
    contextMenu.setTitle("Memoneando");
    
    // Acción para añadir texto
    QAction *addTextAction = contextMenu.addAction("Añadir texto");
    connect(addTextAction, &QAction::triggered, this, &MemeCanvas::addTextLayerRequested);
    
    // Acción para añadir imagen desde archivo
    QAction *addImageAction = contextMenu.addAction("Añadir imagen");
    connect(addImageAction, &QAction::triggered, this, &MemeCanvas::addImageLayerRequested);
    
    // Acción para buscar imágenes online
    QAction *searchImageAction = contextMenu.addAction("Buscar imágenes web");
    connect(searchImageAction, &QAction::triggered, this, &MemeCanvas::searchOnlineImagesRequested);
    
    contextMenu.addSeparator();
    
    // Acción para cambiar color de fondo
    QAction *backgroundAction = contextMenu.addAction("Color de fondo");
    connect(backgroundAction, &QAction::triggered, this, &MemeCanvas::backgroundColorChangeRequested);
    
    // Si hay capas seleccionadas, añadir opciones específicas
    if (!m_selectedLayers.isEmpty()) {
        contextMenu.addSeparator();
        
        QString deleteText = m_selectedLayers.size() > 1 ? 
            QString("Eliminar %1 capas").arg(m_selectedLayers.size()) :
            "Eliminar capa";
        QAction *deleteLayerAction = contextMenu.addAction(deleteText);
        connect(deleteLayerAction, &QAction::triggered, this, [this]() {
            deleteSelectedLayer();
        });
        
        // Si hay solo una capa seleccionada, mostrar opciones específicas
        if (m_selectedLayers.size() == 1) {
            LayerItem* selectedLayer = m_selectedLayers.first();
            
            if (selectedLayer->layerType() == LayerItem::Text) {
                QAction *editTextAction = contextMenu.addAction("Editar texto");
                connect(editTextAction, &QAction::triggered, this, [selectedLayer]() {
                    selectedLayer->startTextEditing();
                });
                
                QAction *changeTextColorAction = contextMenu.addAction("Cambiar color de texto");
                connect(changeTextColorAction, &QAction::triggered, this, &MemeCanvas::textColorChangeRequested);
            }
            
            if (selectedLayer->layerType() == LayerItem::Image) {
                QAction *removeBackgroundAction = contextMenu.addAction("Quitar fondo");
                connect(removeBackgroundAction, &QAction::triggered, this, &MemeCanvas::removeBackgroundRequested);
            }
        } else if (m_selectedLayers.size() > 1) {
            // Opciones para múltiples capas
            QAction *groupMoveAction = contextMenu.addAction("Mover capas juntas");
            groupMoveAction->setEnabled(true); // Siempre habilitado para múltiples capas
        }
    }
    
    // Mostrar el menú en la posición del cursor
    contextMenu.exec(mapToGlobal(position));
}

void MemeCanvas::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete) {
        if (!m_selectedLayers.isEmpty()) {
            deleteSelectedLayer();
            event->accept();
            return;
        }
    }
    
    QGraphicsView::keyPressEvent(event);
}

QPixmap MemeCanvas::loadImageSafely(const QString &imagePath)
{
    // Cargar la imagen como QImage primero para tener control total
    QImageReader reader(imagePath);
    
    // Configurar el reader para evitar problemas con perfiles ICC
    reader.setAutoDetectImageFormat(true);
    
    // Intentar leer la imagen
    QImage image = reader.read();
    
    if (image.isNull()) {
        qWarning() << "No se pudo cargar la imagen:" << imagePath;
        qWarning() << "Error del reader:" << reader.errorString();
        return QPixmap();
    }
    
    // Convertir a un formato estándar para evitar problemas
    // y eliminar cualquier perfil de color problemático
    QImage cleanImage;
    
    if (image.hasAlphaChannel()) {
        // Mantener transparencia si la tiene
        cleanImage = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    } else {
        // Sin transparencia
        cleanImage = image.convertToFormat(QImage::Format_RGB32);
    }
    
    // Limpiar metadatos y perfiles de color
    cleanImage.setColorSpace(QColorSpace());
    
    // Convertir a pixmap
    QPixmap pixmap = QPixmap::fromImage(cleanImage);
    
    if (pixmap.isNull()) {
        qWarning() << "No se pudo convertir la imagen a pixmap:" << imagePath;
        return QPixmap();
    }
    
    qDebug() << "Imagen cargada exitosamente:" << imagePath 
             << "Tamaño:" << pixmap.size() 
             << "Formato original:" << reader.format();
    
    return pixmap;
}

void MemeCanvas::addWatermarkToImage(QPainter &painter)
{
    QSettings settings;
    bool addWatermark = settings.value("export/addWatermark", false).toBool();
    
    if (!addWatermark) {
        return; // No añadir marca de agua si está deshabilitada
    }
    
    QString position = settings.value("export/watermarkPosition", "Esquina inferior derecha").toString();
    
    // Configurar texto de la marca de agua
    QString watermarkText = "Made with Memoneando";
    
    // Configurar fuente para la marca de agua
    QFont watermarkFont("Arial", 12, QFont::Bold);
    painter.setFont(watermarkFont);
    
    // Configurar color con transparencia
    QColor watermarkColor(255, 255, 255, 180); // Blanco semi-transparente
    painter.setPen(watermarkColor);
    
    // Configurar fondo semi-transparente
    QFontMetrics fm(watermarkFont);
    QRect textRect = fm.boundingRect(watermarkText);
    int padding = 8;
    
    // Calcular posición según la configuración
    QPoint textPos;
    QRect canvasRect(0, 0, m_canvasSize.width(), m_canvasSize.height());
    
    if (position == "Esquina inferior derecha") {
        textPos = QPoint(canvasRect.width() - textRect.width() - padding, 
                        canvasRect.height() - padding);
    } else if (position == "Esquina inferior izquierda") {
        textPos = QPoint(padding, canvasRect.height() - padding);
    } else if (position == "Esquina superior derecha") {
        textPos = QPoint(canvasRect.width() - textRect.width() - padding, 
                        textRect.height() + padding);
    } else if (position == "Esquina superior izquierda") {
        textPos = QPoint(padding, textRect.height() + padding);
    } else { // Centro como fallback
        textPos = QPoint((canvasRect.width() - textRect.width()) / 2,
                        (canvasRect.height() + textRect.height()) / 2);
    }
    
    // Dibujar fondo semi-transparente para la marca de agua
    QRect backgroundRect(textPos.x() - padding/2, textPos.y() - textRect.height() - padding/2,
                        textRect.width() + padding, textRect.height() + padding);
    
    QColor backgroundColor(0, 0, 0, 100); // Negro semi-transparente
    painter.fillRect(backgroundRect, backgroundColor);
    
    // Dibujar el texto de la marca de agua
    painter.drawText(textPos, watermarkText);
}

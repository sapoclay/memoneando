#include "layeritem.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QStyleOptionGraphicsItem>
#include <QApplication>
#include <QCursor>
#include <QFontMetrics>
#include <qmath.h>

LayerItem::LayerItem(LayerType type, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , m_type(type)
    , m_font("Arial", 24, QFont::Bold)
    , m_textColor(Qt::black)
    , m_layerName("")
    , m_blendMode(Normal)
    , m_opacity(1.0)
    , m_rotation(0.0)
    , m_itemScale(1.0)
    , m_selected(false)
    , m_hovered(false)
    , m_isEditing(false)
    , m_resizing(false)
    , m_resizeHandle(-1)
    , m_moving(false)
{
    setFlags(QGraphicsItem::ItemIsMovable | 
             QGraphicsItem::ItemIsSelectable | 
             QGraphicsItem::ItemIsFocusable |
             QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    
    if (m_type == Text) {
        m_text = "Texto de ejemplo";
        m_editingText = m_text;
        m_layerName = "Capa de texto";
    } else {
        m_layerName = "Capa de imagen";
    }
    
    updateBoundingRect();
}

LayerItem::~LayerItem()
{
}

void LayerItem::setOpacity(qreal opacity)
{
    m_opacity = qBound(0.0, opacity, 1.0);
    update();
    emit itemChanged();
}

qreal LayerItem::opacity() const
{
    return m_opacity;
}

void LayerItem::setBlendMode(BlendMode mode)
{
    if (m_blendMode != mode) {
        m_blendMode = mode;
        update();
        emit itemChanged();
    }
}

LayerItem::BlendMode LayerItem::blendMode() const
{
    return m_blendMode;
}

void LayerItem::setLayerName(const QString &name)
{
    if (m_layerName != name) {
        m_layerName = name;
        emit itemChanged();
    }
}

QString LayerItem::layerName() const
{
    return m_layerName;
}

void LayerItem::setText(const QString &text)
{
    if (m_type == Text && m_text != text) {
        m_text = text;
        m_editingText = text;
        updateBoundingRect();
        update();
        emit itemChanged();
    }
}

QString LayerItem::text() const
{
    return m_text;
}

void LayerItem::startTextEditing()
{
    if (m_type == Text) {
        m_isEditing = true;
        m_editingText = m_text;
        setFocus();
        update();
    }
}

void LayerItem::finishTextEditing()
{
    if (m_type == Text && m_isEditing) {
        m_isEditing = false;
        setText(m_editingText);
        clearFocus();
        update();
    }
}

void LayerItem::setFont(const QFont &font)
{
    if (m_type == Text && m_font != font) {
        m_font = font;
        updateBoundingRect();
        update();
        emit itemChanged();
    }
}

QFont LayerItem::font() const
{
    return m_font;
}

void LayerItem::setTextColor(const QColor &color)
{
    if (m_type == Text && m_textColor != color) {
        m_textColor = color;
        update();
        emit itemChanged();
    }
}

QColor LayerItem::textColor() const
{
    return m_textColor;
}

void LayerItem::setPixmap(const QPixmap &pixmap)
{
    if (m_type == Image && !pixmap.isNull()) {
        m_pixmap = pixmap;
        updateBoundingRect();
        update();
        emit itemChanged();
    }
}

QPixmap LayerItem::pixmap() const
{
    return m_pixmap;
}

void LayerItem::setRotation(qreal angle)
{
    // Normalizar el ángulo entre -180 y 180
    while (angle > 180) angle -= 360;
    while (angle < -180) angle += 360;
    
    m_rotation = angle;
    setTransformOriginPoint(boundingRect().center());
    QGraphicsItem::setRotation(angle);
    emit itemChanged();
}

qreal LayerItem::rotation() const
{
    return m_rotation;
}

void LayerItem::setScale(qreal scale)
{
    m_itemScale = qMax(0.1, scale);
    setTransformOriginPoint(boundingRect().center());
    QGraphicsItem::setScale(m_itemScale);
    emit itemChanged();
}

qreal LayerItem::scale() const
{
    return m_itemScale;
}

void LayerItem::updateBoundingRect()
{
    prepareGeometryChange();
    
    if (m_type == Text) {
        QFontMetrics metrics(m_font);
        QRectF textRect = metrics.boundingRect(m_text);
        m_boundingRect = textRect.adjusted(-5, -5, 5, 5);
    } else if (m_type == Image && !m_pixmap.isNull()) {
        m_boundingRect = m_pixmap.rect().adjusted(-5, -5, 5, 5);
    } else {
        m_boundingRect = QRectF(0, 0, 100, 50);
    }
    
    // Añadir espacio para los handles de selección
    if (m_selected) {
        m_boundingRect = m_boundingRect.adjusted(-HANDLE_SIZE, -HANDLE_SIZE, HANDLE_SIZE, HANDLE_SIZE);
    }
}

QRectF LayerItem::boundingRect() const
{
    return m_boundingRect;
}

void LayerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    
    painter->setOpacity(m_opacity);
    painter->setRenderHint(QPainter::Antialiasing);
    
    // Configurar modo de mezcla
    switch (m_blendMode) {
        case Normal:
            painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
            break;
        case Multiply:
            painter->setCompositionMode(QPainter::CompositionMode_Multiply);
            break;
        case Screen:
            painter->setCompositionMode(QPainter::CompositionMode_Screen);
            break;
        case Overlay:
            painter->setCompositionMode(QPainter::CompositionMode_Overlay);
            break;
        case SoftLight:
            painter->setCompositionMode(QPainter::CompositionMode_SoftLight);
            break;
        case HardLight:
            painter->setCompositionMode(QPainter::CompositionMode_HardLight);
            break;
    }
    
    QRectF contentRect = m_boundingRect;
    if (m_selected) {
        contentRect = contentRect.adjusted(HANDLE_SIZE, HANDLE_SIZE, -HANDLE_SIZE, -HANDLE_SIZE);
    }
    
    if (m_type == Text) {
        painter->setFont(m_font);
        painter->setPen(m_textColor);
        
        QString displayText = m_isEditing ? m_editingText : m_text;
        painter->drawText(contentRect, Qt::AlignCenter, displayText);
        
        // Si está editando, mostrar cursor
        if (m_isEditing && hasFocus()) {
            QFontMetrics metrics(m_font);
            QRectF textRect = metrics.boundingRect(displayText);
            qreal cursorX = contentRect.x() + (contentRect.width() + textRect.width()) / 2;
            qreal cursorY1 = contentRect.y() + (contentRect.height() - textRect.height()) / 2;
            qreal cursorY2 = cursorY1 + textRect.height();
            
            QPen cursorPen(Qt::black, 2);
            painter->setPen(cursorPen);
            painter->drawLine(QPointF(cursorX, cursorY1), QPointF(cursorX, cursorY2));
        }
        
        // Dibujar borde punteado si está seleccionado
        if (m_selected) {
            QPen pen(m_isEditing ? Qt::red : Qt::blue, 1, Qt::DashLine);
            painter->setPen(pen);
            painter->setBrush(Qt::NoBrush);
            painter->drawRect(contentRect);
        }
    } else if (m_type == Image && !m_pixmap.isNull()) {
        painter->drawPixmap(contentRect.toRect(), m_pixmap);
        
        // Dibujar borde punteado si está seleccionado
        if (m_selected) {
            QPen pen(Qt::blue, 1, Qt::DashLine);
            painter->setPen(pen);
            painter->setBrush(Qt::NoBrush);
            painter->drawRect(contentRect);
        }
    }
    
    // Dibujar handles de redimensionamiento
    if (m_selected) {
        drawSelectionHandles(painter);
    }
}

void LayerItem::drawSelectionHandles(QPainter *painter)
{
    painter->setOpacity(1.0);
    painter->setPen(QPen(Qt::blue, 1));
    painter->setBrush(QBrush(Qt::white));
    
    QRectF contentRect = m_boundingRect.adjusted(HANDLE_SIZE, HANDLE_SIZE, -HANDLE_SIZE, -HANDLE_SIZE);
    
    // Dibujar 8 handles alrededor del rectángulo
    for (int i = 0; i < 8; ++i) {
        QRectF handleRect = getHandleRect(i);
        painter->drawRect(handleRect);
    }
}

QRectF LayerItem::getHandleRect(int handle) const
{
    QRectF contentRect = m_boundingRect.adjusted(HANDLE_SIZE, HANDLE_SIZE, -HANDLE_SIZE, -HANDLE_SIZE);
    qreal x = contentRect.x();
    qreal y = contentRect.y();
    qreal w = contentRect.width();
    qreal h = contentRect.height();
    qreal hs = HANDLE_SIZE;
    
    switch (handle) {
    case TopLeft:     return QRectF(x - hs/2, y - hs/2, hs, hs);
    case TopRight:    return QRectF(x + w - hs/2, y - hs/2, hs, hs);
    case BottomRight: return QRectF(x + w - hs/2, y + h - hs/2, hs, hs);
    case BottomLeft:  return QRectF(x - hs/2, y + h - hs/2, hs, hs);
    case TopMiddle:   return QRectF(x + w/2 - hs/2, y - hs/2, hs, hs);
    case RightMiddle: return QRectF(x + w - hs/2, y + h/2 - hs/2, hs, hs);
    case BottomMiddle:return QRectF(x + w/2 - hs/2, y + h - hs/2, hs, hs);
    case LeftMiddle:  return QRectF(x - hs/2, y + h/2 - hs/2, hs, hs);
    default:          return QRectF();
    }
}

int LayerItem::getHandleAtPoint(const QPointF &point) const
{
    for (int i = 0; i < 8; ++i) {
        if (getHandleRect(i).contains(point)) {
            return i;
        }
    }
    return -1;
}

QPainterPath LayerItem::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void LayerItem::setSelected(bool selected)
{
    if (m_selected != selected) {
        m_selected = selected;
        updateBoundingRect();
        update();
    }
}

bool LayerItem::isSelected() const
{
    return m_selected;
}

QVariant LayerItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged) {
        emit itemChanged();
    }
    return QGraphicsItem::itemChange(change, value);
}

void LayerItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (m_selected) {
            int handle = getHandleAtPoint(event->pos());
            if (handle >= 0) {
                m_resizing = true;
                m_resizeHandle = handle;
                m_resizeStartPos = event->pos();
                m_resizeStartRect = boundingRect();
                return;
            }
        }
        
        m_moving = true;
        m_moveStartPos = event->pos();
        setSelected(true);
        
        // Si está editando y se hace clic fuera del texto, terminar edición
        if (m_isEditing && m_type == Text) {
            finishTextEditing();
        }
    }
    
    QGraphicsItem::mousePressEvent(event);
}

void LayerItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_type == Text && event->button() == Qt::LeftButton) {
        startTextEditing();
    }
    QGraphicsItem::mouseDoubleClickEvent(event);
}

void LayerItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_resizing) {
        QPointF delta = event->pos() - m_resizeStartPos;
        QRectF newRect = m_resizeStartRect;
        
        // Aplicar redimensionamiento según el handle
        switch (m_resizeHandle) {
        case TopLeft:
            newRect.setTopLeft(newRect.topLeft() + delta);
            break;
        case TopRight:
            newRect.setTopRight(newRect.topRight() + delta);
            break;
        case BottomRight:
            newRect.setBottomRight(newRect.bottomRight() + delta);
            break;
        case BottomLeft:
            newRect.setBottomLeft(newRect.bottomLeft() + delta);
            break;
        case TopMiddle:
            newRect.setTop(newRect.top() + delta.y());
            break;
        case RightMiddle:
            newRect.setRight(newRect.right() + delta.x());
            break;
        case BottomMiddle:
            newRect.setBottom(newRect.bottom() + delta.y());
            break;
        case LeftMiddle:
            newRect.setLeft(newRect.left() + delta.x());
            break;
        }
        
        // Aplicar el nuevo tamaño
        if (newRect.width() > 20 && newRect.height() > 20) {
            qreal scaleX = newRect.width() / m_resizeStartRect.width();
            qreal scaleY = newRect.height() / m_resizeStartRect.height();
            
            // Para mantener proporciones en las esquinas
            if (m_resizeHandle <= BottomLeft) {
                qreal scale = qMin(scaleX, scaleY);
                setScale(m_itemScale * scale);
            } else {
                // Para los handles laterales, escalar solo en una dirección
                if (m_resizeHandle == TopMiddle || m_resizeHandle == BottomMiddle) {
                    setScale(m_itemScale * scaleY);
                } else {
                    setScale(m_itemScale * scaleX);
                }
            }
        }
        return;
    }
    
    QGraphicsItem::mouseMoveEvent(event);
}

void LayerItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_resizing = false;
    m_moving = false;
    m_resizeHandle = -1;
    
    QGraphicsItem::mouseReleaseEvent(event);
}

void LayerItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_hovered = true;
    updateCursor(event->pos());
    QGraphicsItem::hoverEnterEvent(event);
}

void LayerItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hovered = false;
    setCursor(Qt::ArrowCursor);
    QGraphicsItem::hoverLeaveEvent(event);
}

void LayerItem::updateCursor(const QPointF &pos)
{
    if (!m_selected) {
        setCursor(Qt::ArrowCursor);
        return;
    }
    
    int handle = getHandleAtPoint(pos);
    switch (handle) {
    case TopLeft:
    case BottomRight:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case TopRight:
    case BottomLeft:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case TopMiddle:
    case BottomMiddle:
        setCursor(Qt::SizeVerCursor);
        break;
    case LeftMiddle:
    case RightMiddle:
        setCursor(Qt::SizeHorCursor);
        break;
    default:
        setCursor(Qt::SizeAllCursor);
        break;
    }
}

void LayerItem::keyPressEvent(QKeyEvent *event)
{
    if (m_type == Text && m_isEditing) {
        switch (event->key()) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
        case Qt::Key_Escape:
            finishTextEditing();
            break;
        case Qt::Key_Backspace:
            if (!m_editingText.isEmpty()) {
                m_editingText.chop(1);
                update();
            }
            break;
        default:
            if (!event->text().isEmpty() && event->text()[0].isPrint()) {
                m_editingText += event->text();
                update();
            }
            break;
        }
        event->accept();
    } else {
        QGraphicsItem::keyPressEvent(event);
    }
}

void LayerItem::removeBackground(const QColor &backgroundColor, int tolerance)
{
    if (m_type != Image || m_pixmap.isNull()) {
        return;
    }
    
    QPixmap newPixmap = createBackgroundRemovalPreview(backgroundColor, tolerance);
    if (!newPixmap.isNull()) {
        setPixmap(newPixmap);
        emit itemChanged();
    }
}

QPixmap LayerItem::createBackgroundRemovalPreview(const QColor &backgroundColor, int tolerance) const
{
    if (m_type != Image || m_pixmap.isNull()) {
        return QPixmap();
    }
    
    QImage originalImage = m_pixmap.toImage();
    if (originalImage.isNull()) {
        return QPixmap();
    }
    
    // Convertir a ARGB32 para soporte de transparencia
    QImage resultImage = originalImage.convertToFormat(QImage::Format_ARGB32);
    
    int targetRed = backgroundColor.red();
    int targetGreen = backgroundColor.green();
    int targetBlue = backgroundColor.blue();
    
    // Recorrer todos los píxeles
    for (int y = 0; y < resultImage.height(); ++y) {
        for (int x = 0; x < resultImage.width(); ++x) {
            QColor pixelColor(resultImage.pixel(x, y));
            
            // Calcular la diferencia de color
            int redDiff = qAbs(pixelColor.red() - targetRed);
            int greenDiff = qAbs(pixelColor.green() - targetGreen);
            int blueDiff = qAbs(pixelColor.blue() - targetBlue);
            
            // Si el color está dentro de la tolerancia, hacerlo transparente
            if (redDiff <= tolerance && greenDiff <= tolerance && blueDiff <= tolerance) {
                // Hacer el píxel completamente transparente
                resultImage.setPixel(x, y, qRgba(0, 0, 0, 0));
            }
        }
    }
    
    return QPixmap::fromImage(resultImage);
}

#include "colorpickerwidget.h"
#include <QDebug>
#include <QApplication>

ColorPickerWidget::ColorPickerWidget(QWidget *parent)
    : QLabel(parent)
    , m_selectedColor(Qt::white)
    , m_showCrosshair(false)
    , m_mousePressed(false)
    , m_colorLocked(false)
{
    setMinimumSize(200, 200);
    setScaledContents(true);
    setAlignment(Qt::AlignCenter);
    setStyleSheet("QLabel { border: 2px solid #666; background-color: #f0f0f0; }");
    setCursor(Qt::CrossCursor);
    setMouseTracking(true); // Para detectar movimiento sin clicks
}

void ColorPickerWidget::setPixmap(const QPixmap &pixmap)
{
    m_originalPixmap = pixmap;
    QLabel::setPixmap(pixmap);
}

void ColorPickerWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && !m_originalPixmap.isNull()) {
        QPoint imagePos = getImagePosition(event->pos());
        QColor color = getColorAtPosition(imagePos);
        
        if (color.isValid()) {
            m_selectedColor = color;
            m_selectedPosition = event->pos();
            m_lastMousePos = event->pos();
            m_showCrosshair = true;
            m_colorLocked = true; // Bloquear el color después del clic
            update();
            emit colorSelected(color);
        }
    }
    QLabel::mousePressEvent(event);
}

void ColorPickerWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_originalPixmap.isNull()) {
        QPoint imagePos = getImagePosition(event->pos());
        QColor color = getColorAtPosition(imagePos);
        
        if (color.isValid()) {
            // Solo actualizar la posición visual del cursor si no tenemos un color bloqueado
            if (!m_colorLocked) {
                m_lastMousePos = event->pos();
                m_showCrosshair = true;
            } else {
                // Si tenemos color bloqueado, mantener la posición donde se hizo clic
                m_lastMousePos = m_selectedPosition;
                m_showCrosshair = true;
            }
            
            // Siempre emitir hover para mostrar información, pero no cambiar el color seleccionado si está bloqueado
            emit colorHovered(color);
            
            update();
        }
    }
    QLabel::mouseMoveEvent(event);
}

void ColorPickerWidget::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    
    if (m_showCrosshair && !m_lastMousePos.isNull()) {
        QPainter painter(this);
        drawCrosshair(painter, m_lastMousePos);
    }
}

void ColorPickerWidget::leaveEvent(QEvent *event)
{
    // Solo ocultar crosshair si no tenemos un color bloqueado
    if (!m_colorLocked) {
        m_showCrosshair = false;
    }
    m_mousePressed = false;
    update();
    QLabel::leaveEvent(event);
}

QColor ColorPickerWidget::getColorAtPosition(const QPoint &pos) const
{
    if (m_originalPixmap.isNull() || pos.x() < 0 || pos.y() < 0) {
        return QColor();
    }
    
    // Convertir a imagen para acceder a píxeles
    QImage image = m_originalPixmap.toImage();
    
    if (pos.x() >= image.width() || pos.y() >= image.height()) {
        return QColor();
    }
    
    QRgb rgb = image.pixel(pos.x(), pos.y());
    return QColor(rgb);
}

QPoint ColorPickerWidget::getImagePosition(const QPoint &widgetPos) const
{
    if (m_originalPixmap.isNull()) {
        return QPoint(-1, -1);
    }
    
    // Obtener el rect donde se dibuja la imagen escalada
    QSize labelSize = size();
    QSize imageSize = m_originalPixmap.size();
    
    // Calcular la escala manteniendo la proporción
    qreal scaleX = static_cast<qreal>(labelSize.width()) / imageSize.width();
    qreal scaleY = static_cast<qreal>(labelSize.height()) / imageSize.height();
    qreal scale = qMin(scaleX, scaleY);
    
    // Calcular el tamaño real de la imagen escalada
    QSize scaledSize = imageSize * scale;
    
    // Calcular el offset para centrar la imagen
    int offsetX = (labelSize.width() - scaledSize.width()) / 2;
    int offsetY = (labelSize.height() - scaledSize.height()) / 2;
    
    // Convertir la posición del widget a la posición en la imagen original
    int imageX = static_cast<int>((widgetPos.x() - offsetX) / scale);
    int imageY = static_cast<int>((widgetPos.y() - offsetY) / scale);
    
    return QPoint(imageX, imageY);
}

void ColorPickerWidget::drawCrosshair(QPainter &painter, const QPoint &center)
{
    painter.save();
    
    // Configurar el painter
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPen pen(Qt::red, 2);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    
    // Dibujar líneas del crosshair
    int crossSize = 20;
    
    // Línea horizontal
    painter.drawLine(center.x() - crossSize, center.y(), center.x() + crossSize, center.y());
    
    // Línea vertical
    painter.drawLine(center.x(), center.y() - crossSize, center.x(), center.y() + crossSize);
    
    // Dibujar un círculo en el centro
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(center, 5, 5);
    
    // Dibujar un círculo más grande con el color seleccionado
    if (m_selectedColor.isValid()) {
        painter.setBrush(m_selectedColor);
        pen.setColor(Qt::white);
        pen.setWidth(2);
        painter.setPen(pen);
        painter.drawEllipse(center.x() - 15, center.y() - 25, 30, 20);
    }
    
    painter.restore();
}

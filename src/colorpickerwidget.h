#ifndef COLORPICKERWIDGET_H
#define COLORPICKERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QColor>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPoint>

class ColorPickerWidget : public QLabel
{
    Q_OBJECT

public:
    explicit ColorPickerWidget(QWidget *parent = nullptr);
    
    void setPixmap(const QPixmap &pixmap);
    QColor selectedColor() const { return m_selectedColor; }
    void setSelectedColor(const QColor &color) { m_selectedColor = color; m_colorLocked = true; update(); }
    void resetSelection() { m_colorLocked = false; m_showCrosshair = false; update(); }
    
    void setCrosshairVisible(bool visible) { m_showCrosshair = visible; update(); }
    
signals:
    void colorSelected(const QColor &color);
    void colorHovered(const QColor &color);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QColor getColorAtPosition(const QPoint &pos) const;
    QPoint getImagePosition(const QPoint &widgetPos) const;
    void drawCrosshair(QPainter &painter, const QPoint &center);
    
    QPixmap m_originalPixmap;
    QColor m_selectedColor;
    QPoint m_lastMousePos;
    QPoint m_selectedPosition; // Posición donde se hizo clic para seleccionar el color
    bool m_showCrosshair;
    bool m_mousePressed;
    bool m_colorLocked; // Indica si el color está bloqueado después de un clic
};

#endif // COLORPICKERWIDGET_H

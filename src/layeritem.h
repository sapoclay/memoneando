#ifndef LAYERITEM_H
#define LAYERITEM_H

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QFont>
#include <QColor>
#include <QPainter>
#include <QPixmap>
#include <QKeyEvent>

class LayerItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    enum LayerType {
        Text,
        Image
    };
    
    enum BlendMode {
        Normal,
        Multiply,
        Screen,
        Overlay,
        SoftLight,
        HardLight
    };

    explicit LayerItem(LayerType type, QGraphicsItem *parent = nullptr);
    ~LayerItem();

    // Propiedades básicas
    LayerType layerType() const { return m_type; }
    void setOpacity(qreal opacity);
    qreal opacity() const;
    void setBlendMode(BlendMode mode);
    BlendMode blendMode() const;
    void setLayerName(const QString &name);
    QString layerName() const;
    
    // Propiedades de texto
    void setText(const QString &text);
    QString text() const;
    void setFont(const QFont &font);
    QFont font() const;
    void setTextColor(const QColor &color);
    QColor textColor() const;
    void startTextEditing();
    void finishTextEditing();
    
    // Propiedades de imagen
    void setPixmap(const QPixmap &pixmap);
    QPixmap pixmap() const;
    
    // Eliminación de fondo
    void removeBackground(const QColor &backgroundColor, int tolerance = 30);
    QPixmap createBackgroundRemovalPreview(const QColor &backgroundColor, int tolerance = 30) const;
    
    // Transformaciones
    void setRotation(qreal angle);
    qreal rotation() const;
    void setScale(qreal scale);
    qreal scale() const;
    
    // QGraphicsItem interface
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QPainterPath shape() const override;
    
    // Interacción
    void setSelected(bool selected);
    bool isSelected() const;

signals:
    void itemChanged();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void updateBoundingRect();
    void drawSelectionHandles(QPainter *painter);
    QRectF getHandleRect(int handle) const;
    int getHandleAtPoint(const QPointF &point) const;
    void updateCursor(const QPointF &pos);

    LayerType m_type;
    QString m_text;
    QFont m_font;
    QColor m_textColor;
    QPixmap m_pixmap;
    QString m_layerName;
    BlendMode m_blendMode;
    
    qreal m_opacity;
    qreal m_rotation;
    qreal m_itemScale;
    bool m_selected;
    bool m_hovered;
    
    QRectF m_boundingRect;
    
    // Para edición de texto
    bool m_isEditing;
    QString m_editingText;
    
    // Para redimensionamiento
    bool m_resizing;
    int m_resizeHandle;
    QPointF m_resizeStartPos;
    QRectF m_resizeStartRect;
    
    // Para movimiento
    bool m_moving;
    QPointF m_moveStartPos;
    
    static const int HANDLE_SIZE = 8;
    enum HandlePosition {
        TopLeft = 0,
        TopRight,
        BottomRight,
        BottomLeft,
        TopMiddle,
        RightMiddle,
        BottomMiddle,
        LeftMiddle
    };
};

#endif // LAYERITEM_H

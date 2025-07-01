#ifndef MEMECANVAS_H
#define MEMECANVAS_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPixmap>
#include <QList>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QGraphicsProxyWidget>
#include <QContextMenuEvent>
#include <QMenu>

class LayerItem;

class MemeCanvas : public QGraphicsView
{
    Q_OBJECT

public:
    explicit MemeCanvas(QWidget *parent = nullptr);
    ~MemeCanvas();

    // Gestión del canvas
    void setCanvasSize(const QSize &size);
    QSize canvasSize() const;
    void setZoom(qreal factor);
    qreal zoom() const;
    void setCanvasBackgroundColor(const QColor &color);
    QColor canvasBackgroundColor() const;
    
    // Gestión de capas
    void addTextLayer(const QString &text);
    void addImageLayer(const QString &imagePath);
    void deleteSelectedLayer();
    void clearCanvas();
    void loadBackgroundImage(const QString &imagePath);
    
    // Exportación
    bool exportImage(const QString &fileName);
    
    // Información de capas
    QStringList getLayers() const;
    LayerItem* getSelectedLayer() const;
    QList<LayerItem*> getSelectedLayers() const;
    void setSelectedLayers(const QList<LayerItem*> &layers);
    void addToSelection(LayerItem* layer);
    void removeFromSelection(LayerItem* layer);
    void clearSelection();
    void reorderLayers(const QStringList &newOrder);
    LayerItem* getLayerAt(int index) const;

public slots:
    void selectLayer(LayerItem *layer);

signals:
    void layerSelected(LayerItem *layer);
    void layersSelectionChanged(const QList<LayerItem*> &layers);
    void layersChanged();
    void backgroundColorChangeRequested();
    void addTextLayerRequested();
    void addImageLayerRequested();
    void searchOnlineImagesRequested();
    void removeBackgroundRequested();
    void textColorChangeRequested();

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onSelectionChanged();

private:
    void updateZOrder();
    void setupScene();
    void updateCanvasRect();
    void centerCanvas();
    void createContextMenu(const QPoint &position);
    LayerItem* createTextLayer(const QString &text, const QPointF &pos);
    LayerItem* createImageLayer(const QPixmap &pixmap, const QPointF &pos);
    void addWatermarkToImage(QPainter &painter);
    
    // Helper para cargar imágenes de forma segura
    QPixmap loadImageSafely(const QString &imagePath);

    QGraphicsScene *m_scene;
    QGraphicsRectItem *m_canvasRect;
    QList<LayerItem*> m_layers;
    QList<LayerItem*> m_selectedLayers;
    
    QSize m_canvasSize;
    qreal m_zoomFactor;
    QPixmap m_backgroundImage;
    QGraphicsPixmapItem *m_backgroundItem;
    QColor m_canvasBackgroundColor;
    
    // Para el arrastre del canvas
    bool m_dragging;
    QPoint m_lastPanPoint;
};

#endif // MEMECANVAS_H

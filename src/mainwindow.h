#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QComboBox>
#include <QListWidget>
#include <QSplitter>
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QInputDialog>

class MemeCanvas;
class SystemTrayIcon;
class SettingsDialog;
class LayerItem;
class ImageSearchWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void newMeme();
    void openImage();
    void saveMeme();
    void saveMemeAs();
    void exportMeme();
    void showSettings();
    void addTextLayer();
    void addImageLayer();
    void searchOnlineImages();
    void deleteCurrentLayer();
    void onLayerSelectionChanged();
    void onLayersSelectionChanged(const QList<LayerItem*> &layers);
    void removeBackground();
    void moveLayerUp();
    void moveLayerDown();
    void onCanvasSizeChanged();
    void onLayerRotationChanged();
    void onLayerOpacityChanged();
    void onLayerBlendModeChanged();
    void changeCanvasBackgroundColor();
    void onLayerReordered();
    void onLayerDoubleClicked(QListWidgetItem *item);
    void changeTextColor();
    void showAbout();

private:
    void setupUI();
    void setupSidePanel();
    void setupCanvasArea();
    void setupPropertiesPanel();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void createActions();
    void updateLayerList();
    void setupSystemTray();
    void updateBackgroundColorButton();
    void updatePropertiesPanel(LayerItem* layer);
    void updatePropertiesPanelForMultipleSelection(const QList<LayerItem*> &layers);
    void setPropertiesPanelEnabled(bool enabled);

    // Widgets principales
    MemeCanvas *m_canvas;
    SystemTrayIcon *m_trayIcon;
    SettingsDialog *m_settingsDialog;
    
    // Panel lateral
    QWidget *m_sidePanel;
    QListWidget *m_layerList;
    QPushButton *m_addTextBtn;
    QPushButton *m_addImageBtn;
    QPushButton *m_searchImageBtn;
    QPushButton *m_deleteLayerBtn;
    QPushButton *m_moveUpBtn;
    QPushButton *m_moveDownBtn;
    QSlider *m_opacitySlider;
    QComboBox *m_blendModeCombo;
    QPushButton *m_backgroundColorBtn;
    QLineEdit *m_defaultTextEdit;
    
    // Panel de propiedades
    QWidget *m_propertiesPanel;
    QSpinBox *m_xPosSpinBox;
    QSpinBox *m_yPosSpinBox;
    QSpinBox *m_widthSpinBox;
    QSpinBox *m_heightSpinBox;
    QSlider *m_rotationSlider;
    
    // Panel de canvas
    QScrollArea *m_scrollArea;
    QComboBox *m_canvasSizeCombo;
    QSlider *m_zoomSlider;
    
    // Acciones del menú
    QAction *m_newAction;
    QAction *m_openAction;
    QAction *m_saveAction;
    QAction *m_saveAsAction;
    QAction *m_exportAction;
    QAction *m_exitAction;
    QAction *m_settingsAction;
    QAction *m_aboutAction;
    QAction *m_removeBackgroundAction;
    
    // Variables de estado
    QString m_currentFileName;
    bool m_hasUnsavedChanges;
};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "memecanvas.h"
#include "systemtrayicon.h"
#include "settingsdialog.h"
#include "layeritem.h"
#include "imagesearchwidget.h"

#include <QApplication>
#include <QCloseEvent>
#include <QStandardPaths>
#include <QDir>
#include <QTextEdit>
#include <QSplitter>
#include <QGroupBox>
#include <QGridLayout>
#include <QFormLayout>
#include <QDesktopServices>
#include <QUrl>
#include <QPixmap>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_canvas(nullptr)
    , m_trayIcon(nullptr)
    , m_settingsDialog(nullptr)
    , m_hasUnsavedChanges(false)
{
    setWindowTitle(tr("Memoneando - Editor de Memes"));
    setWindowIcon(QIcon(":/icons/logo.png"));
    resize(1200, 800);
    
    setupUI();
    createActions();
    setupMenuBar();
    setupToolBar();
    setupStatusBar();
    setupSystemTray();
    
    // Conectar señales del canvas
    connect(m_canvas, &MemeCanvas::layersSelectionChanged, this, &MainWindow::onLayersSelectionChanged);
    connect(m_canvas, &MemeCanvas::layersChanged, this, &MainWindow::updateLayerList);
    connect(m_canvas, &MemeCanvas::backgroundColorChangeRequested, this, &MainWindow::changeCanvasBackgroundColor);
    connect(m_canvas, &MemeCanvas::addTextLayerRequested, this, &MainWindow::addTextLayer);
    connect(m_canvas, &MemeCanvas::addImageLayerRequested, this, &MainWindow::addImageLayer);
    connect(m_canvas, &MemeCanvas::searchOnlineImagesRequested, this, &MainWindow::searchOnlineImages);
    connect(m_canvas, &MemeCanvas::removeBackgroundRequested, this, &MainWindow::removeBackground);
    connect(m_canvas, &MemeCanvas::textColorChangeRequested, this, &MainWindow::changeTextColor);
}

MainWindow::~MainWindow()
{
    delete m_settingsDialog;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_hasUnsavedChanges) {
        int ret = QMessageBox::warning(this, tr("Memoneando"),
                     tr("Hay cambios sin guardar.\n"
                        "¿Quieres guardar el meme antes de cerrar?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                     QMessageBox::Save);
        
        if (ret == QMessageBox::Save) {
            saveMeme();
        } else if (ret == QMessageBox::Cancel) {
            event->ignore();
            return;
        }
    }
    
    hide();
    event->ignore();
}

void MainWindow::newMeme()
{
    if (m_hasUnsavedChanges) {
        int ret = QMessageBox::warning(this, tr("Nuevo Meme"),
                     tr("Hay cambios sin guardar.\n"
                        "¿Quieres guardar el meme actual antes de crear uno nuevo?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                     QMessageBox::Save);
        
        if (ret == QMessageBox::Save) {
            saveMeme();
        } else if (ret == QMessageBox::Cancel) {
            return;
        }
    }
    
    m_canvas->clearCanvas();
    m_currentFileName.clear();
    m_hasUnsavedChanges = false;
    updateLayerList();
    setWindowTitle(tr("Memoneando - Editor de Memes"));
}

void MainWindow::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Abrir Imagen"), QString(),
        tr("Archivos de Imagen (*.png *.jpg *.jpeg *.bmp *.gif *.tiff)"));
    
    if (!fileName.isEmpty()) {
        m_canvas->loadBackgroundImage(fileName);
        m_hasUnsavedChanges = true;
        updateLayerList();
    }
}

void MainWindow::saveMeme()
{
    if (m_currentFileName.isEmpty()) {
        saveMemeAs();
    } else {
        if (m_canvas->exportImage(m_currentFileName)) {
            m_hasUnsavedChanges = false;
            statusBar()->showMessage(tr("Meme guardado exitosamente"), 2000);
        } else {
            QMessageBox::warning(this, tr("Error"), tr("No se pudo guardar el meme."));
        }
    }
}

void MainWindow::saveMemeAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Guardar Meme Como"), QString(),
        tr("Archivos PNG (*.png);;Archivos JPEG (*.jpg);;Todos los archivos (*)"));
    
    if (!fileName.isEmpty()) {
        if (m_canvas->exportImage(fileName)) {
            m_currentFileName = fileName;
            m_hasUnsavedChanges = false;
            setWindowTitle(tr("Memoneando - %1").arg(QFileInfo(fileName).baseName()));
            statusBar()->showMessage(tr("Meme guardado exitosamente"), 2000);
        } else {
            QMessageBox::warning(this, tr("Error"), tr("No se pudo guardar el meme."));
        }
    }
}

void MainWindow::exportMeme()
{
    saveMemeAs();
}

void MainWindow::showSettings()
{
    if (!m_settingsDialog) {
        m_settingsDialog = new SettingsDialog(this);
    }
    m_settingsDialog->exec();
}

void MainWindow::addTextLayer()
{
    QString text = m_defaultTextEdit->text();
    if (text.isEmpty()) {
        text = tr("Texto del meme");
    }
    
    m_canvas->addTextLayer(text);
    m_hasUnsavedChanges = true;
    updateLayerList();
}

void MainWindow::addImageLayer()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Añadir Imagen"), QString(),
        tr("Archivos de Imagen (*.png *.jpg *.jpeg *.bmp *.gif *.tiff)"));
    
    if (!fileName.isEmpty()) {
        m_canvas->addImageLayer(fileName);
        m_hasUnsavedChanges = true;
        updateLayerList();
    }
}

void MainWindow::searchOnlineImages()
{
    ImageSearchWidget *searchWidget = new ImageSearchWidget(this);
    connect(searchWidget, &ImageSearchWidget::imageSelected, [this](const QString &imagePath) {
        m_canvas->addImageLayer(imagePath);
        m_hasUnsavedChanges = true;
        updateLayerList();
    });
    
    searchWidget->show();
}

void MainWindow::deleteCurrentLayer()
{
    m_canvas->deleteSelectedLayer();
    m_hasUnsavedChanges = true;
    updateLayerList();
}

void MainWindow::onLayerSelectionChanged()
{
    updateLayerList();
    LayerItem* layer = m_canvas->getSelectedLayer();
    updatePropertiesPanel(layer);
}

void MainWindow::onLayersSelectionChanged(const QList<LayerItem*> &layers)
{
    if (layers.size() == 1) {
        updatePropertiesPanel(layers.first());
    } else if (layers.size() > 1) {
        updatePropertiesPanelForMultipleSelection(layers);
    } else {
        setPropertiesPanelEnabled(false);
    }
}

void MainWindow::removeBackground()
{
    // Implementar función de remover fondo
    QMessageBox::information(this, tr("Función en desarrollo"), 
                           tr("La función de remover fondo estará disponible en una próxima versión."));
}

void MainWindow::moveLayerUp()
{
    // Implementar mover capa hacia arriba
}

void MainWindow::moveLayerDown()
{
    // Implementar mover capa hacia abajo
}

void MainWindow::onCanvasSizeChanged()
{
    // Actualizar la información del canvas
}

void MainWindow::onLayerRotationChanged()
{
    // Implementar cambio de rotación
}

void MainWindow::onLayerOpacityChanged()
{
    // Implementar cambio de opacidad
}

void MainWindow::onLayerBlendModeChanged()
{
    // Implementar cambio de modo de mezcla
}

void MainWindow::changeCanvasBackgroundColor()
{
    QColor color = QColorDialog::getColor(m_canvas->canvasBackgroundColor(), this,
                                         tr("Seleccionar Color de Fondo"));
    
    if (color.isValid()) {
        m_canvas->setCanvasBackgroundColor(color);
        updateBackgroundColorButton();
        m_hasUnsavedChanges = true;
    }
}

void MainWindow::onLayerReordered()
{
    updateLayerList();
    m_hasUnsavedChanges = true;
}

void MainWindow::onLayerDoubleClicked(QListWidgetItem *item)
{
    Q_UNUSED(item)
    // Implementar edición de capa al hacer doble clic
}

void MainWindow::changeTextColor()
{
    QColor color = QColorDialog::getColor(Qt::black, this, tr("Seleccionar Color de Texto"));
    
    if (color.isValid()) {
        LayerItem* layer = m_canvas->getSelectedLayer();
        if (layer) {
            // Implementar cambio de color de texto
            m_hasUnsavedChanges = true;
        }
    }
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, tr("Acerca de Memoneando"),
                      tr("<h2>Memoneando 1.0.8</h2>"
                         "<p>Editor de memes fácil de usar</p>"
                         "<p>Desarrollado por el equipo de Memoneando</p>"
                         "<p>¡Diviértete creando memes!</p>"));
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget;
    setCentralWidget(centralWidget);
    
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    
    // Crear splitter principal
    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal);
    mainLayout->addWidget(mainSplitter);
    
    setupSidePanel();
    setupCanvasArea();
    setupPropertiesPanel();
    
    mainSplitter->addWidget(m_sidePanel);
    mainSplitter->addWidget(m_scrollArea);
    mainSplitter->addWidget(m_propertiesPanel);
    
    mainSplitter->setSizes({200, 600, 200});
}

void MainWindow::setupSidePanel()
{
    m_sidePanel = new QWidget;
    m_sidePanel->setMaximumWidth(250);
    
    QVBoxLayout *layout = new QVBoxLayout(m_sidePanel);
    
    // Grupo de acciones
    QGroupBox *actionsGroup = new QGroupBox(tr("Acciones"));
    QVBoxLayout *actionsLayout = new QVBoxLayout(actionsGroup);
    
    m_addTextBtn = new QPushButton(tr("Añadir Texto"));
    m_addImageBtn = new QPushButton(tr("Añadir Imagen"));
    m_searchImageBtn = new QPushButton(tr("Buscar Imágenes"));
    m_deleteLayerBtn = new QPushButton(tr("Eliminar Capa"));
    
    actionsLayout->addWidget(m_addTextBtn);
    actionsLayout->addWidget(m_addImageBtn);
    actionsLayout->addWidget(m_searchImageBtn);
    actionsLayout->addWidget(m_deleteLayerBtn);
    
    // Texto por defecto
    m_defaultTextEdit = new QLineEdit(tr("Texto del meme"));
    actionsLayout->addWidget(new QLabel(tr("Texto por defecto:")));
    actionsLayout->addWidget(m_defaultTextEdit);
    
    layout->addWidget(actionsGroup);
    
    // Lista de capas
    QGroupBox *layersGroup = new QGroupBox(tr("Capas"));
    QVBoxLayout *layersLayout = new QVBoxLayout(layersGroup);
    
    m_layerList = new QListWidget;
    layersLayout->addWidget(m_layerList);
    
    QHBoxLayout *layerButtonsLayout = new QHBoxLayout;
    m_moveUpBtn = new QPushButton(tr("↑"));
    m_moveDownBtn = new QPushButton(tr("↓"));
    layerButtonsLayout->addWidget(m_moveUpBtn);
    layerButtonsLayout->addWidget(m_moveDownBtn);
    layersLayout->addLayout(layerButtonsLayout);
    
    layout->addWidget(layersGroup);
    
    // Controles de capa
    QGroupBox *layerControlsGroup = new QGroupBox(tr("Controles"));
    QFormLayout *controlsLayout = new QFormLayout(layerControlsGroup);
    
    m_opacitySlider = new QSlider(Qt::Horizontal);
    m_opacitySlider->setRange(0, 100);
    m_opacitySlider->setValue(100);
    controlsLayout->addRow(tr("Opacidad:"), m_opacitySlider);
    
    m_blendModeCombo = new QComboBox;
    m_blendModeCombo->addItems({tr("Normal"), tr("Multiplicar"), tr("Pantalla"), tr("Superposición")});
    controlsLayout->addRow(tr("Modo de mezcla:"), m_blendModeCombo);
    
    m_backgroundColorBtn = new QPushButton(tr("Color de fondo"));
    controlsLayout->addRow(m_backgroundColorBtn);
    
    layout->addWidget(layerControlsGroup);
    
    layout->addStretch();
    
    // Conectar señales
    connect(m_addTextBtn, &QPushButton::clicked, this, &MainWindow::addTextLayer);
    connect(m_addImageBtn, &QPushButton::clicked, this, &MainWindow::addImageLayer);
    connect(m_searchImageBtn, &QPushButton::clicked, this, &MainWindow::searchOnlineImages);
    connect(m_deleteLayerBtn, &QPushButton::clicked, this, &MainWindow::deleteCurrentLayer);
    connect(m_moveUpBtn, &QPushButton::clicked, this, &MainWindow::moveLayerUp);
    connect(m_moveDownBtn, &QPushButton::clicked, this, &MainWindow::moveLayerDown);
    connect(m_backgroundColorBtn, &QPushButton::clicked, this, &MainWindow::changeCanvasBackgroundColor);
    connect(m_layerList, &QListWidget::itemDoubleClicked, this, &MainWindow::onLayerDoubleClicked);
}

void MainWindow::setupCanvasArea()
{
    m_canvas = new MemeCanvas;
    m_scrollArea = new QScrollArea;
    m_scrollArea->setWidget(m_canvas);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setAlignment(Qt::AlignCenter);
}

void MainWindow::setupPropertiesPanel()
{
    m_propertiesPanel = new QWidget;
    m_propertiesPanel->setMaximumWidth(250);
    
    QVBoxLayout *layout = new QVBoxLayout(m_propertiesPanel);
    
    QGroupBox *transformGroup = new QGroupBox(tr("Transformación"));
    QFormLayout *transformLayout = new QFormLayout(transformGroup);
    
    m_xPosSpinBox = new QSpinBox;
    m_xPosSpinBox->setRange(-9999, 9999);
    transformLayout->addRow(tr("X:"), m_xPosSpinBox);
    
    m_yPosSpinBox = new QSpinBox;
    m_yPosSpinBox->setRange(-9999, 9999);
    transformLayout->addRow(tr("Y:"), m_yPosSpinBox);
    
    m_widthSpinBox = new QSpinBox;
    m_widthSpinBox->setRange(1, 9999);
    transformLayout->addRow(tr("Ancho:"), m_widthSpinBox);
    
    m_heightSpinBox = new QSpinBox;
    m_heightSpinBox->setRange(1, 9999);
    transformLayout->addRow(tr("Alto:"), m_heightSpinBox);
    
    m_rotationSlider = new QSlider(Qt::Horizontal);
    m_rotationSlider->setRange(-180, 180);
    m_rotationSlider->setValue(0);
    transformLayout->addRow(tr("Rotación:"), m_rotationSlider);
    
    layout->addWidget(transformGroup);
    
    QGroupBox *canvasGroup = new QGroupBox(tr("Canvas"));
    QFormLayout *canvasLayout = new QFormLayout(canvasGroup);
    
    m_canvasSizeCombo = new QComboBox;
    m_canvasSizeCombo->addItems({tr("800x600"), tr("1024x768"), tr("1280x720"), tr("1920x1080")});
    canvasLayout->addRow(tr("Tamaño:"), m_canvasSizeCombo);
    
    m_zoomSlider = new QSlider(Qt::Horizontal);
    m_zoomSlider->setRange(10, 500);
    m_zoomSlider->setValue(100);
    canvasLayout->addRow(tr("Zoom:"), m_zoomSlider);
    
    layout->addWidget(canvasGroup);
    
    layout->addStretch();
    
    setPropertiesPanelEnabled(false);
}

void MainWindow::setupMenuBar()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&Archivo"));
    fileMenu->addAction(m_newAction);
    fileMenu->addAction(m_openAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_saveAction);
    fileMenu->addAction(m_saveAsAction);
    fileMenu->addAction(m_exportAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_exitAction);
    
    QMenu *editMenu = menuBar()->addMenu(tr("&Editar"));
    editMenu->addAction(m_removeBackgroundAction);
    
    QMenu *toolsMenu = menuBar()->addMenu(tr("&Herramientas"));
    toolsMenu->addAction(m_settingsAction);
    
    QMenu *helpMenu = menuBar()->addMenu(tr("&Ayuda"));
    helpMenu->addAction(m_aboutAction);
}

void MainWindow::setupToolBar()
{
    QToolBar *toolBar = addToolBar(tr("Principal"));
    toolBar->addAction(m_newAction);
    toolBar->addAction(m_openAction);
    toolBar->addAction(m_saveAction);
    toolBar->addSeparator();
    toolBar->addAction(m_settingsAction);
}

void MainWindow::setupStatusBar()
{
    statusBar()->showMessage(tr("Listo"));
}

void MainWindow::createActions()
{
    m_newAction = new QAction(tr("&Nuevo"), this);
    m_newAction->setShortcut(QKeySequence::New);
    m_newAction->setStatusTip(tr("Crear un nuevo meme"));
    connect(m_newAction, &QAction::triggered, this, &MainWindow::newMeme);
    
    m_openAction = new QAction(tr("&Abrir"), this);
    m_openAction->setShortcut(QKeySequence::Open);
    m_openAction->setStatusTip(tr("Abrir una imagen"));
    connect(m_openAction, &QAction::triggered, this, &MainWindow::openImage);
    
    m_saveAction = new QAction(tr("&Guardar"), this);
    m_saveAction->setShortcut(QKeySequence::Save);
    m_saveAction->setStatusTip(tr("Guardar el meme"));
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::saveMeme);
    
    m_saveAsAction = new QAction(tr("Guardar &como..."), this);
    m_saveAsAction->setShortcut(QKeySequence::SaveAs);
    m_saveAsAction->setStatusTip(tr("Guardar el meme con un nuevo nombre"));
    connect(m_saveAsAction, &QAction::triggered, this, &MainWindow::saveMemeAs);
    
    m_exportAction = new QAction(tr("&Exportar"), this);
    m_exportAction->setStatusTip(tr("Exportar el meme"));
    connect(m_exportAction, &QAction::triggered, this, &MainWindow::exportMeme);
    
    m_exitAction = new QAction(tr("&Salir"), this);
    m_exitAction->setShortcut(QKeySequence::Quit);
    m_exitAction->setStatusTip(tr("Salir de la aplicación"));
    connect(m_exitAction, &QAction::triggered, this, &QWidget::close);
    
    m_settingsAction = new QAction(tr("&Configuración"), this);
    m_settingsAction->setStatusTip(tr("Abrir configuración"));
    connect(m_settingsAction, &QAction::triggered, this, &MainWindow::showSettings);
    
    m_aboutAction = new QAction(tr("&Acerca de"), this);
    m_aboutAction->setStatusTip(tr("Acerca de Memoneando"));
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
    
    m_removeBackgroundAction = new QAction(tr("&Remover fondo"), this);
    m_removeBackgroundAction->setStatusTip(tr("Remover el fondo de una imagen"));
    connect(m_removeBackgroundAction, &QAction::triggered, this, &MainWindow::removeBackground);
}

void MainWindow::updateLayerList()
{
    m_layerList->clear();
    QStringList layers = m_canvas->getLayers();
    m_layerList->addItems(layers);
}

void MainWindow::setupSystemTray()
{
    m_trayIcon = new SystemTrayIcon(this);
    m_trayIcon->show();
}

void MainWindow::updateBackgroundColorButton()
{
    QColor color = m_canvas->canvasBackgroundColor();
    QString style = QString("background-color: %1; border: 1px solid #000;").arg(color.name());
    m_backgroundColorBtn->setStyleSheet(style);
}

void MainWindow::updatePropertiesPanel(LayerItem* layer)
{
    if (layer) {
        setPropertiesPanelEnabled(true);
        // Actualizar los valores de las propiedades
    } else {
        setPropertiesPanelEnabled(false);
    }
}

void MainWindow::updatePropertiesPanelForMultipleSelection(const QList<LayerItem*> &layers)
{
    Q_UNUSED(layers)
    setPropertiesPanelEnabled(true);
    // Implementar lógica para selección múltiple
}

void MainWindow::setPropertiesPanelEnabled(bool enabled)
{
    m_propertiesPanel->setEnabled(enabled);
}

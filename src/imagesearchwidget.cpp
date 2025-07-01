#include "imagesearchwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QLabel>
#include <QProgressBar>
#include <QTimer>
#include <QMessageBox>
#include <QMouseEvent>
#include <QApplication>
#include <QStyle>
#include <QFrame>
#include <QComboBox>
#include "pixabayprovider.h"
#include "unsplashprovider.h"
#include "pexelsprovider.h"
#include "wikimediaprovider.h"
#include "lorempicsumprovider.h"

ImageSearchWidget::ImageSearchWidget(QWidget *parent)
    : QWidget(parent)
    , m_searchLineEdit(nullptr)
    , m_providerComboBox(nullptr)
    , m_searchButton(nullptr)
    , m_resultsScrollArea(nullptr)
    , m_resultsWidget(nullptr)
    , m_resultsLayout(nullptr)
    , m_statusLabel(nullptr)
    , m_infoLabel(nullptr)
    , m_progressBar(nullptr)
    , m_downloader(new ImageDownloader(this))
    , m_searchTimer(new QTimer(this))
    , m_currentResults(new QList<ImageSearchResult>())
    , m_currentProvider(nullptr)
{
    setupUI();
    setupProviders();
    
    // Configurar timer para búsqueda retrasada
    m_searchTimer->setSingleShot(true);
    m_searchTimer->setInterval(500); // 500ms delay
    
    // Conectar señales
    connect(m_searchButton, &QPushButton::clicked, this, &ImageSearchWidget::onSearchClicked);
    connect(m_searchLineEdit, &QLineEdit::textChanged, this, &ImageSearchWidget::onSearchTextChanged);
    connect(m_searchLineEdit, &QLineEdit::returnPressed, this, &ImageSearchWidget::onSearchClicked);
    connect(m_searchTimer, &QTimer::timeout, this, &ImageSearchWidget::onSearchClicked);
    connect(m_providerComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ImageSearchWidget::onProviderChanged);
    
    connect(m_downloader, &ImageDownloader::imageDownloaded, this, &ImageSearchWidget::onImageDownloaded);
    connect(m_downloader, &ImageDownloader::imageDownloadError, this, &ImageSearchWidget::onImageDownloadError);
    connect(m_downloader, &ImageDownloader::previewReady, this, &ImageSearchWidget::onPreviewReady);
}

void ImageSearchWidget::setupUI()
{
    setWindowTitle("Buscar Imágenes Gratuitas - Memoneando");
    setMinimumSize(800, 600);
    resize(900, 700);
    
    // Configurar ventana como independiente
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Área de búsqueda
    QHBoxLayout *searchLayout = new QHBoxLayout();
    
    // Selector de proveedor
    m_providerComboBox = new QComboBox();
    m_providerComboBox->setMinimumWidth(150);
    
    m_searchLineEdit = new QLineEdit();
    m_searchLineEdit->setPlaceholderText("Buscar imágenes gratuitas (ej: gato, naturaleza, tecnología...)");
    
    m_searchButton = new QPushButton("Buscar");
    m_searchButton->setDefault(true);
    
    searchLayout->addWidget(new QLabel("Proveedor:"));
    searchLayout->addWidget(m_providerComboBox);
    searchLayout->addWidget(m_searchLineEdit);
    searchLayout->addWidget(m_searchButton);
    
    mainLayout->addLayout(searchLayout);
    
    // Barra de estado y progreso
    m_statusLabel = new QLabel("Selecciona un proveedor e ingresa un término de búsqueda");
    m_progressBar = new QProgressBar();
    m_progressBar->setVisible(false);
    
    mainLayout->addWidget(m_statusLabel);
    mainLayout->addWidget(m_progressBar);
    
    // Área de resultados
    m_resultsScrollArea = new QScrollArea();
    m_resultsScrollArea->setWidgetResizable(true);
    m_resultsScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    m_resultsWidget = new QWidget();
    m_resultsLayout = new QGridLayout(m_resultsWidget);
    m_resultsLayout->setSpacing(10);
    
    m_resultsScrollArea->setWidget(m_resultsWidget);
    mainLayout->addWidget(m_resultsScrollArea);
    
    // Información de pie
    m_infoLabel = new QLabel();
    m_infoLabel->setStyleSheet("color: gray; font-size: 10px;");
    m_infoLabel->setAlignment(Qt::AlignCenter);
    m_infoLabel->setWordWrap(true);
    mainLayout->addWidget(m_infoLabel);
    
    updateInfoLabel();
}

void ImageSearchWidget::onSearchClicked()
{
    QString query = m_searchLineEdit->text().trimmed();
    if (query.isEmpty()) {
        QMessageBox::information(this, "Búsqueda", "Por favor ingresa un término de búsqueda");
        return;
    }
    
    if (!m_currentProvider) {
        QMessageBox::warning(this, "Búsqueda", "Por favor selecciona un proveedor de imágenes");
        return;
    }
    
    if (!m_currentProvider->isAvailable()) {
        QMessageBox::warning(this, "Búsqueda", 
            QString("El proveedor %1 no está disponible. Verifica la configuración.")
            .arg(m_currentProvider->getDisplayName()));
        return;
    }
    
    clearResults();
    // Desconectar señales del proveedor anterior si existe
    if (m_currentProvider) {
        disconnect(m_currentProvider, &ImageProvider::searchCompleted, 
                  this, &ImageSearchWidget::onSearchCompleted);
        disconnect(m_currentProvider, &ImageProvider::searchError, 
                  this, &ImageSearchWidget::onSearchError);
    }
    
    showLoading(true);
    m_statusLabel->setText(QString("Buscando en %1: %2...")
                          .arg(m_currentProvider->getDisplayName())
                          .arg(query));
    
    // Conectar señales del proveedor actual
    connect(m_currentProvider, &ImageProvider::searchCompleted, 
            this, &ImageSearchWidget::onSearchCompleted);
    connect(m_currentProvider, &ImageProvider::searchError, 
            this, &ImageSearchWidget::onSearchError);
    
    m_currentProvider->searchImages(query);
}

void ImageSearchWidget::onSearchTextChanged()
{
    // Reiniciar timer para búsqueda retrasada
    m_searchTimer->stop();
    if (!m_searchLineEdit->text().trimmed().isEmpty()) {
        m_searchTimer->start();
    }
}

void ImageSearchWidget::onSearchCompleted(const QList<ImageSearchResult> &results)
{
    showLoading(false);
    
    if (results.isEmpty()) {
        m_statusLabel->setText("No se encontraron imágenes. Intenta con otros términos de búsqueda.");
        return;
    }
    
    QString providerName = m_currentProvider ? m_currentProvider->getDisplayName() : "Proveedor";
    m_statusLabel->setText(QString("Se encontraron %1 imágenes en %2")
                          .arg(results.size())
                          .arg(providerName));
    
    *m_currentResults = results;
    
    // Crear widgets para los resultados
    for (int i = 0; i < results.size(); ++i) {
        createResultWidget(results[i]);
    }
    
    // Cargar previews
    for (const auto &result : results) {
        m_downloader->getImagePreview(result.previewUrl);
    }
}

void ImageSearchWidget::onSearchError(const QString &error)
{
    showLoading(false);
    m_statusLabel->setText(QString("Error en búsqueda: %1").arg(error));
    QMessageBox::warning(this, "Error de búsqueda", error);
}

void ImageSearchWidget::onImageDownloaded(const QString &filePath)
{
    m_statusLabel->setText("Imagen descargada exitosamente");
    emit imageSelected(filePath);
    
    // Cerrar la ventana después de seleccionar imagen
    if (window()) {
        window()->close();
    }
}

void ImageSearchWidget::onImageDownloadError(const QString &error)
{
    m_statusLabel->setText(QString("Error descargando imagen: %1").arg(error));
    QMessageBox::warning(this, "Error de descarga", error);
}

void ImageSearchWidget::onImageResultClicked(const ImageSearchResult &result)
{
    m_statusLabel->setText("Descargando imagen...");
    // Usar la URL de imagen completa en lugar de la URL web
    m_downloader->downloadImage(result.fullImageUrl);
}

void ImageSearchWidget::onPreviewReady(const QPixmap &preview, const QString &imageUrl)
{
    // Buscar el widget correspondiente y actualizar su preview
    for (auto it = m_resultWidgets.begin(); it != m_resultWidgets.end(); ++it) {
        ImageResultWidget *widget = it.value();
        if (widget->getResult().previewUrl == imageUrl) {
            widget->setPreviewPixmap(preview);
            break;
        }
    }
}

void ImageSearchWidget::clearResults()
{
    // Limpiar widgets de resultados anteriores
    QLayoutItem *item;
    while ((item = m_resultsLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    m_resultWidgets.clear();
}

void ImageSearchWidget::showLoading(bool loading)
{
    m_progressBar->setVisible(loading);
    m_searchButton->setEnabled(!loading);
    
    if (loading) {
        m_progressBar->setRange(0, 0); // Indicador de progreso indeterminado
    }
}

void ImageSearchWidget::createResultWidget(const ImageSearchResult &result)
{
    ImageResultWidget *resultWidget = new ImageResultWidget(result, this);
    
    connect(resultWidget, &ImageResultWidget::clicked, this, &ImageSearchWidget::onImageResultClicked);
    
    // Calcular posición en grid
    int itemCount = m_resultsLayout->count();
    int row = itemCount / RESULTS_PER_ROW;
    int col = itemCount % RESULTS_PER_ROW;
    
    m_resultsLayout->addWidget(resultWidget, row, col);
    m_resultWidgets[result.previewUrl] = resultWidget;
}

void ImageSearchWidget::setupProviders()
{
    // Crear y añadir todos los proveedores disponibles
    m_providers.append(new PixabayProvider(this));
    m_providers.append(new UnsplashProvider(this));
    m_providers.append(new PexelsProvider(this));
    m_providers.append(new WikimediaProvider(this));
    m_providers.append(new LoremPicsumProvider(this));
    
    // Configurar el combo box con los proveedores
    for (ImageProvider *provider : m_providers) {
        QString displayText = provider->getDisplayName();
        if (!provider->isAvailable()) {
            displayText += " (No disponible)";
        }
        m_providerComboBox->addItem(displayText, provider->getName());
    }
    
    // Seleccionar el primer proveedor disponible por defecto
    for (int i = 0; i < m_providers.size(); ++i) {
        if (m_providers[i]->isAvailable()) {
            m_providerComboBox->setCurrentIndex(i);
            m_currentProvider = m_providers[i];
            break;
        }
    }
    
    // Actualizar información del proveedor
    updateInfoLabel();
}

void ImageSearchWidget::onProviderChanged()
{
    int index = m_providerComboBox->currentIndex();
    if (index >= 0 && index < m_providers.size()) {
        // Desconectar del proveedor anterior si existe
        if (m_currentProvider) {
            disconnect(m_currentProvider, &ImageProvider::searchCompleted, 
                      this, &ImageSearchWidget::onSearchCompleted);
            disconnect(m_currentProvider, &ImageProvider::searchError, 
                      this, &ImageSearchWidget::onSearchError);
        }
        
        m_currentProvider = m_providers[index];
        updateInfoLabel();
        clearResults(); // Limpiar resultados anteriores
        
        if (!m_currentProvider->isAvailable()) {
            m_statusLabel->setText(QString("El proveedor %1 no está disponible").arg(m_currentProvider->getDisplayName()));
        } else {
            m_statusLabel->setText(QString("Proveedor seleccionado: %1").arg(m_currentProvider->getDisplayName()));
        }
    }
}

void ImageSearchWidget::updateInfoLabel()
{
    if (!m_currentProvider) {
        m_infoLabel->setText("Selecciona un proveedor para comenzar");
        return;
    }
    
    QString infoText;
    QString providerName = m_currentProvider->getDisplayName();
    
    if (m_currentProvider->getName() == "pixabay") {
        infoText = QString("Imágenes de %1 - Libres para uso comercial y personal").arg(providerName);
    } else if (m_currentProvider->getName() == "unsplash") {
        infoText = QString("Imágenes de %1 - Fotografías profesionales libres (requiere API key)").arg(providerName);
    } else if (m_currentProvider->getName() == "pexels") {
        infoText = QString("Imágenes de %1 - Fotografías y videos gratuitos (requiere API key)").arg(providerName);
    } else if (m_currentProvider->getName() == "wikimedia") {
        infoText = QString("Imágenes de %1 - Contenido de dominio público y Creative Commons").arg(providerName);
    } else if (m_currentProvider->getName() == "lorempicsum") {
        infoText = QString("Imágenes de %1 - Fotografías placeholder para desarrollo").arg(providerName);
    } else {
        infoText = QString("Imágenes proporcionadas por %1").arg(providerName);
    }
    
    if (!m_currentProvider->isAvailable()) {
        infoText += " - ⚠️ No disponible (verifica configuración)";
    }
    
    m_infoLabel->setText(infoText);
}

// ImageResultWidget implementation

ImageResultWidget::ImageResultWidget(const ImageSearchResult &result, QWidget *parent)
    : QWidget(parent)
    , m_result(result)
    , m_previewLabel(nullptr)
    , m_infoLabel(nullptr)
    , m_hovered(false)
{
    setFixedSize(180, 220);
    setCursor(Qt::PointingHandCursor);
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    
    // Preview de la imagen
    m_previewLabel = new QLabel();
    m_previewLabel->setFixedSize(170, 150);
    m_previewLabel->setAlignment(Qt::AlignCenter);
    m_previewLabel->setStyleSheet("border: 1px solid #ddd; background-color: #f9f9f9;");
    m_previewLabel->setText("Cargando...");
    
    // Información de la imagen
    m_infoLabel = new QLabel();
    m_infoLabel->setWordWrap(true);
    m_infoLabel->setMaximumHeight(60);
    m_infoLabel->setStyleSheet("font-size: 11px; color: #666;");
    
    QString infoText = QString("Vistas: %1 | Descargas: %2\n%3 x %4\nPor: %5\nProveedor: %6")
                          .arg(m_result.views)
                          .arg(m_result.downloads)
                          .arg(m_result.width)
                          .arg(m_result.height)
                          .arg(m_result.user)
                          .arg(m_result.provider);
    
    m_infoLabel->setText(infoText);
    
    layout->addWidget(m_previewLabel);
    layout->addWidget(m_infoLabel);
    
    updateStyle();
}

void ImageResultWidget::setPreviewPixmap(const QPixmap &pixmap)
{
    if (!pixmap.isNull()) {
        QPixmap scaledPixmap = pixmap.scaled(170, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        m_previewLabel->setPixmap(scaledPixmap);
    }
}

void ImageResultWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked(m_result);
    }
    QWidget::mousePressEvent(event);
}

void ImageResultWidget::enterEvent(QEnterEvent *event)
{
    m_hovered = true;
    updateStyle();
    QWidget::enterEvent(event);
}

void ImageResultWidget::leaveEvent(QEvent *event)
{
    m_hovered = false;
    updateStyle();
    QWidget::leaveEvent(event);
}

void ImageResultWidget::updateStyle()
{
    QString style = "QWidget { border: 2px solid %1; border-radius: 5px; background-color: %2; }";
    
    if (m_hovered) {
        setStyleSheet(style.arg("#4CAF50", "#f0f8ff"));
    } else {
        setStyleSheet(style.arg("#ddd", "white"));
    }
}

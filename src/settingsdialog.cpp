#include "settingsdialog.h"
#include <QSettings>
#include <QApplication>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QFormLayout>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , m_defaultTextColor(Qt::black)
    , m_defaultFont("Arial", 24, QFont::Bold)
    , m_canvasBackgroundColor(Qt::white)
    , m_defaultText("Texto de ejemplo")
{
    setWindowTitle("Configuración - Memoneando");
    setMinimumSize(500, 400);
    setModal(true);
    
    auto mainLayout = new QVBoxLayout(this);
    
    // Crear widget de pestañas
    m_tabWidget = new QTabWidget;
    
    setupGeneralTab();
    setupAppearanceTab();
    setupExportTab();
    setupApiKeysTab();
    
    m_tabWidget->addTab(m_generalTab, "General");
    m_tabWidget->addTab(m_appearanceTab, "Apariencia");
    m_tabWidget->addTab(m_exportTab, "Exportación");
    m_tabWidget->addTab(m_apiKeysTab, "API Keys");
    
    mainLayout->addWidget(m_tabWidget);
    
    // Botones
    auto buttonLayout = new QHBoxLayout;
    m_resetBtn = new QPushButton("Restablecer");
    m_applyBtn = new QPushButton("Aplicar");
    m_cancelBtn = new QPushButton("Cancelar");
    
    buttonLayout->addWidget(m_resetBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_applyBtn);
    buttonLayout->addWidget(m_cancelBtn);
    
    mainLayout->addLayout(buttonLayout);
    
    // Conectar señales
    connect(m_applyBtn, &QPushButton::clicked, this, &SettingsDialog::applySettings);
    connect(m_cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_resetBtn, &QPushButton::clicked, this, &SettingsDialog::resetSettings);
    
    loadSettings();
}

void SettingsDialog::setupGeneralTab()
{
    m_generalTab = new QWidget;
    auto layout = new QVBoxLayout(m_generalTab);
    
    // Grupo de inicio
    auto startupGroup = new QGroupBox("Inicio de la aplicación");
    auto startupLayout = new QVBoxLayout(startupGroup);
    
    m_startMinimizedCheck = new QCheckBox("Iniciar minimizado en la bandeja");
    m_showTrayIconCheck = new QCheckBox("Mostrar icono en la bandeja del sistema");
    m_showTrayIconCheck->setChecked(true);
    m_showTrayIconCheck->setEnabled(false); // Siempre habilitado por requisito
    
    startupLayout->addWidget(m_startMinimizedCheck);
    startupLayout->addWidget(m_showTrayIconCheck);
    
    layout->addWidget(startupGroup);
    
    // Grupo de guardado automático
    auto autoSaveGroup = new QGroupBox("Guardado automático");
    auto autoSaveLayout = new QVBoxLayout(autoSaveGroup);
    
    m_autoSaveCheck = new QCheckBox("Habilitar guardado automático");
    autoSaveLayout->addWidget(m_autoSaveCheck);
    
    auto intervalLayout = new QHBoxLayout;
    intervalLayout->addWidget(new QLabel("Intervalo (minutos):"));
    m_autoSaveIntervalSpin = new QSpinBox;
    m_autoSaveIntervalSpin->setRange(1, 60);
    m_autoSaveIntervalSpin->setValue(5);
    intervalLayout->addWidget(m_autoSaveIntervalSpin);
    intervalLayout->addStretch();
    
    autoSaveLayout->addLayout(intervalLayout);
    
    layout->addWidget(autoSaveGroup);
    layout->addStretch();
}

void SettingsDialog::setupAppearanceTab()
{
    m_appearanceTab = new QWidget;
    auto layout = new QVBoxLayout(m_appearanceTab);
    
    // Grupo de tema
    auto themeGroup = new QGroupBox("Tema");
    auto themeLayout = new QFormLayout(themeGroup);
    
    m_themeCombo = new QComboBox;
    m_themeCombo->addItems({"Claro", "Oscuro", "Sistema"});
    themeLayout->addRow("Tema de la interfaz:", m_themeCombo);
    
    layout->addWidget(themeGroup);
    
    // Grupo de colores y fuentes por defecto
    auto defaultsGroup = new QGroupBox("Valores por defecto");
    auto defaultsLayout = new QFormLayout(defaultsGroup);
    
    m_textColorBtn = new QPushButton("Seleccionar color");
    connect(m_textColorBtn, &QPushButton::clicked, this, &SettingsDialog::chooseDefaultTextColor);
    defaultsLayout->addRow("Color de texto:", m_textColorBtn);
    
    m_fontBtn = new QPushButton("Seleccionar fuente");
    connect(m_fontBtn, &QPushButton::clicked, this, &SettingsDialog::chooseDefaultFont);
    defaultsLayout->addRow("Fuente de texto:", m_fontBtn);
    
    m_canvasColorBtn = new QPushButton("Seleccionar color");
    connect(m_canvasColorBtn, &QPushButton::clicked, this, &SettingsDialog::chooseCanvasBackgroundColor);
    defaultsLayout->addRow("Fondo del canvas:", m_canvasColorBtn);
    
    m_defaultTextEdit = new QLineEdit;
    m_defaultTextEdit->setPlaceholderText("Ingrese el texto por defecto para nuevas capas");
    defaultsLayout->addRow("Texto por defecto:", m_defaultTextEdit);
    
    layout->addWidget(defaultsGroup);
    
    // Grupo de escala de UI
    auto scaleGroup = new QGroupBox("Escala de la interfaz");
    auto scaleLayout = new QVBoxLayout(scaleGroup);
    
    auto scaleSliderLayout = new QHBoxLayout;
    scaleSliderLayout->addWidget(new QLabel("Pequeño"));
    m_uiScaleSlider = new QSlider(Qt::Horizontal);
    m_uiScaleSlider->setRange(80, 150);
    m_uiScaleSlider->setValue(100);
    scaleSliderLayout->addWidget(m_uiScaleSlider);
    scaleSliderLayout->addWidget(new QLabel("Grande"));
    
    scaleLayout->addLayout(scaleSliderLayout);
    layout->addWidget(scaleGroup);
    
    layout->addStretch();
}

void SettingsDialog::setupExportTab()
{
    m_exportTab = new QWidget;
    auto layout = new QVBoxLayout(m_exportTab);
    
    // Grupo de formato por defecto
    auto formatGroup = new QGroupBox("Formato de exportación");
    auto formatLayout = new QFormLayout(formatGroup);
    
    m_defaultFormatCombo = new QComboBox;
    m_defaultFormatCombo->addItems({"PNG", "JPEG", "BMP", "TIFF"});
    formatLayout->addRow("Formato por defecto:", m_defaultFormatCombo);
    
    auto qualityLayout = new QHBoxLayout;
    qualityLayout->addWidget(new QLabel("Baja"));
    m_jpegQualitySlider = new QSlider(Qt::Horizontal);
    m_jpegQualitySlider->setRange(10, 100);
    m_jpegQualitySlider->setValue(90);
    qualityLayout->addWidget(m_jpegQualitySlider);
    qualityLayout->addWidget(new QLabel("Alta"));
    
    formatLayout->addRow("Calidad JPEG:", qualityLayout);
    
    layout->addWidget(formatGroup);
    
    // Grupo de marca de agua
    auto watermarkGroup = new QGroupBox("Marca de agua");
    auto watermarkLayout = new QVBoxLayout(watermarkGroup);
    
    m_addWatermarkCheck = new QCheckBox("Añadir marca de agua 'Memoneando'");
    watermarkLayout->addWidget(m_addWatermarkCheck);
    
    auto positionLayout = new QHBoxLayout;
    positionLayout->addWidget(new QLabel("Posición:"));
    m_watermarkPositionCombo = new QComboBox;
    m_watermarkPositionCombo->addItems({"Esquina inferior derecha", "Esquina inferior izquierda", 
                                       "Esquina superior derecha", "Esquina superior izquierda"});
    positionLayout->addWidget(m_watermarkPositionCombo);
    positionLayout->addStretch();
    
    watermarkLayout->addLayout(positionLayout);
    layout->addWidget(watermarkGroup);
    
    layout->addStretch();
}

void SettingsDialog::setupApiKeysTab()
{
    m_apiKeysTab = new QWidget;
    auto layout = new QVBoxLayout(m_apiKeysTab);
    
    // Información general
    auto infoLabel = new QLabel(
        "Configura las API keys para acceder a más proveedores de imágenes gratuitas.\n"
        "Todas las API keys son completamente gratuitas."
    );
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("color: #666; font-size: 11px; margin-bottom: 10px;");
    layout->addWidget(infoLabel);
    
    // Unsplash API Key
    auto unsplashGroup = new QGroupBox("Unsplash (Fotografías profesionales)");
    auto unsplashLayout = new QVBoxLayout(unsplashGroup);
    
    auto unsplashInfoLabel = new QLabel(
        "Obtén tu Access Key gratuita en: https://unsplash.com/developers\n"
        "Registra una aplicación y copia tu Access Key."
    );
    unsplashInfoLabel->setWordWrap(true);
    unsplashInfoLabel->setStyleSheet("font-size: 10px; color: #888;");
    unsplashLayout->addWidget(unsplashInfoLabel);
    
    auto unsplashFormLayout = new QFormLayout;
    m_unsplashApiKeyEdit = new QLineEdit;
    m_unsplashApiKeyEdit->setEchoMode(QLineEdit::Password);
    m_unsplashApiKeyEdit->setPlaceholderText("Ingresa tu Unsplash Access Key");
    
    auto unsplashButtonLayout = new QHBoxLayout;
    m_testUnsplashBtn = new QPushButton("Probar");
    m_unsplashStatusLabel = new QLabel("No configurado");
    m_unsplashStatusLabel->setStyleSheet("color: #888;");
    
    unsplashButtonLayout->addWidget(m_testUnsplashBtn);
    unsplashButtonLayout->addWidget(m_unsplashStatusLabel);
    unsplashButtonLayout->addStretch();
    
    unsplashFormLayout->addRow("Access Key:", m_unsplashApiKeyEdit);
    unsplashLayout->addLayout(unsplashFormLayout);
    unsplashLayout->addLayout(unsplashButtonLayout);
    
    layout->addWidget(unsplashGroup);
    
    // Pexels API Key
    auto pexelsGroup = new QGroupBox("Pexels (Contenido variado)");
    auto pexelsLayout = new QVBoxLayout(pexelsGroup);
    
    auto pexelsInfoLabel = new QLabel(
        "Obtén tu API Key gratuita en: https://www.pexels.com/api/\n"
        "Genera tu API key gratuita después del registro."
    );
    pexelsInfoLabel->setWordWrap(true);
    pexelsInfoLabel->setStyleSheet("font-size: 10px; color: #888;");
    pexelsLayout->addWidget(pexelsInfoLabel);
    
    auto pexelsFormLayout = new QFormLayout;
    m_pexelsApiKeyEdit = new QLineEdit;
    m_pexelsApiKeyEdit->setEchoMode(QLineEdit::Password);
    m_pexelsApiKeyEdit->setPlaceholderText("Ingresa tu Pexels API Key");
    
    auto pexelsButtonLayout = new QHBoxLayout;
    m_testPexelsBtn = new QPushButton("Probar");
    m_pexelsStatusLabel = new QLabel("No configurado");
    m_pexelsStatusLabel->setStyleSheet("color: #888;");
    
    pexelsButtonLayout->addWidget(m_testPexelsBtn);
    pexelsButtonLayout->addWidget(m_pexelsStatusLabel);
    pexelsButtonLayout->addStretch();
    
    pexelsFormLayout->addRow("API Key:", m_pexelsApiKeyEdit);
    pexelsLayout->addLayout(pexelsFormLayout);
    pexelsLayout->addLayout(pexelsButtonLayout);
    
    layout->addWidget(pexelsGroup);
    
    // Proveedores sin API key
    auto freeGroup = new QGroupBox("Proveedores sin API key requerida");
    auto freeLayout = new QVBoxLayout(freeGroup);
    
    auto freeInfoLabel = new QLabel(
        "• Pixabay: Funciona inmediatamente sin configuración\n"
        "• Wikimedia Commons: Acceso libre a contenido de dominio público\n"
        "• Lorem Picsum: Imágenes placeholder para desarrollo"
    );
    freeInfoLabel->setStyleSheet("color: #666; font-size: 11px;");
    freeLayout->addWidget(freeInfoLabel);
    
    layout->addWidget(freeGroup);
    
    layout->addStretch();
    
    // Conectar señales
    connect(m_testUnsplashBtn, &QPushButton::clicked, this, &SettingsDialog::testUnsplashApiKey);
    connect(m_testPexelsBtn, &QPushButton::clicked, this, &SettingsDialog::testPexelsApiKey);
    connect(m_unsplashApiKeyEdit, &QLineEdit::textChanged, this, &SettingsDialog::saveApiKeys);
    connect(m_pexelsApiKeyEdit, &QLineEdit::textChanged, this, &SettingsDialog::saveApiKeys);
    
    // Cargar API keys existentes
    loadApiKeys();
}

void SettingsDialog::loadSettings()
{
    QSettings settings;
    
    // General
    m_startMinimizedCheck->setChecked(settings.value("general/startMinimized", false).toBool());
    m_autoSaveCheck->setChecked(settings.value("general/autoSave", true).toBool());
    m_autoSaveIntervalSpin->setValue(settings.value("general/autoSaveInterval", 5).toInt());
    
    // Apariencia
    m_themeCombo->setCurrentText(settings.value("appearance/theme", "Sistema").toString());
    m_defaultTextColor = settings.value("appearance/defaultTextColor", QColor(Qt::black)).value<QColor>();
    m_defaultFont = settings.value("appearance/defaultFont", QFont("Arial", 24, QFont::Bold)).value<QFont>();
    m_canvasBackgroundColor = settings.value("appearance/canvasBackground", QColor(Qt::white)).value<QColor>();
    m_defaultText = settings.value("appearance/defaultText", "Texto de ejemplo").toString();
    m_defaultTextEdit->setText(m_defaultText);
    m_uiScaleSlider->setValue(settings.value("appearance/uiScale", 100).toInt());
    
    // Exportación
    m_defaultFormatCombo->setCurrentText(settings.value("export/defaultFormat", "PNG").toString());
    m_jpegQualitySlider->setValue(settings.value("export/jpegQuality", 90).toInt());
    m_addWatermarkCheck->setChecked(settings.value("export/addWatermark", false).toBool());
    m_watermarkPositionCombo->setCurrentText(settings.value("export/watermarkPosition", "Esquina inferior derecha").toString());
    
    // API Keys
    m_unsplashApiKeyEdit->setText(settings.value("apiKeys/unsplash", "").toString());
    m_pexelsApiKeyEdit->setText(settings.value("apiKeys/pexels", "").toString());
    
    // Actualizar botones de color
    QString colorStyle = QString("background-color: %1; border: 1px solid black;").arg(m_defaultTextColor.name());
    m_textColorBtn->setStyleSheet(colorStyle);
    
    QString canvasColorStyle = QString("background-color: %1; border: 1px solid black;").arg(m_canvasBackgroundColor.name());
    m_canvasColorBtn->setStyleSheet(canvasColorStyle);
    
    m_fontBtn->setText(QString("%1, %2pt").arg(m_defaultFont.family()).arg(m_defaultFont.pointSize()));
}

void SettingsDialog::saveSettings()
{
    QSettings settings;
    
    // General
    settings.setValue("general/startMinimized", m_startMinimizedCheck->isChecked());
    settings.setValue("general/autoSave", m_autoSaveCheck->isChecked());
    settings.setValue("general/autoSaveInterval", m_autoSaveIntervalSpin->value());
    
    // Apariencia
    settings.setValue("appearance/theme", m_themeCombo->currentText());
    settings.setValue("appearance/defaultTextColor", m_defaultTextColor);
    settings.setValue("appearance/defaultFont", m_defaultFont);
    settings.setValue("appearance/canvasBackground", m_canvasBackgroundColor);
    settings.setValue("appearance/defaultText", m_defaultTextEdit->text());
    settings.setValue("appearance/uiScale", m_uiScaleSlider->value());
    
    // Exportación
    settings.setValue("export/defaultFormat", m_defaultFormatCombo->currentText());
    settings.setValue("export/jpegQuality", m_jpegQualitySlider->value());
    settings.setValue("export/addWatermark", m_addWatermarkCheck->isChecked());
    settings.setValue("export/watermarkPosition", m_watermarkPositionCombo->currentText());
    
    // API Keys
    settings.setValue("apiKeys/unsplash", m_unsplashApiKeyEdit->text().trimmed());
    settings.setValue("apiKeys/pexels", m_pexelsApiKeyEdit->text().trimmed());
}

void SettingsDialog::applySettings()
{
    // Actualizar texto por defecto antes de guardar
    m_defaultText = m_defaultTextEdit->text();
    saveSettings();
    accept();
}

void SettingsDialog::resetSettings()
{
    // Restablecer valores por defecto
    m_startMinimizedCheck->setChecked(false);
    m_autoSaveCheck->setChecked(true);
    m_autoSaveIntervalSpin->setValue(5);
    
    m_themeCombo->setCurrentText("Sistema");
    m_defaultTextColor = Qt::black;
    m_defaultFont = QFont("Arial", 24, QFont::Bold);
    m_canvasBackgroundColor = Qt::white;
    m_defaultText = "Texto de ejemplo";
    m_defaultTextEdit->setText(m_defaultText);
    m_uiScaleSlider->setValue(100);
    
    m_defaultFormatCombo->setCurrentText("PNG");
    m_jpegQualitySlider->setValue(90);
    m_addWatermarkCheck->setChecked(false);
    m_watermarkPositionCombo->setCurrentText("Esquina inferior derecha");
    
    // Actualizar UI
    QString colorStyle = QString("background-color: %1; border: 1px solid black;").arg(m_defaultTextColor.name());
    m_textColorBtn->setStyleSheet(colorStyle);
    
    QString canvasColorStyle = QString("background-color: %1; border: 1px solid black;").arg(m_canvasBackgroundColor.name());
    m_canvasColorBtn->setStyleSheet(canvasColorStyle);
    
    m_fontBtn->setText(QString("%1, %2pt").arg(m_defaultFont.family()).arg(m_defaultFont.pointSize()));
}

void SettingsDialog::chooseDefaultTextColor()
{
    QColor color = QColorDialog::getColor(m_defaultTextColor, this, "Seleccionar color de texto");
    if (color.isValid()) {
        m_defaultTextColor = color;
        QString colorStyle = QString("background-color: %1; border: 1px solid black;").arg(color.name());
        m_textColorBtn->setStyleSheet(colorStyle);
    }
}

void SettingsDialog::chooseDefaultFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, m_defaultFont, this, "Seleccionar fuente");
    if (ok) {
        m_defaultFont = font;
        m_fontBtn->setText(QString("%1, %2pt").arg(font.family()).arg(font.pointSize()));
    }
}

void SettingsDialog::chooseCanvasBackgroundColor()
{
    QColor color = QColorDialog::getColor(m_canvasBackgroundColor, this, "Seleccionar color de fondo");
    if (color.isValid()) {
        m_canvasBackgroundColor = color;
        QString colorStyle = QString("background-color: %1; border: 1px solid black;").arg(color.name());
        m_canvasColorBtn->setStyleSheet(colorStyle);
    }
}

void SettingsDialog::testUnsplashApiKey()
{
    QString apiKey = m_unsplashApiKeyEdit->text().trimmed();
    if (apiKey.isEmpty()) {
        m_unsplashStatusLabel->setText("API key vacía");
        m_unsplashStatusLabel->setStyleSheet("color: red;");
        return;
    }
    
    m_unsplashStatusLabel->setText("Probando...");
    m_unsplashStatusLabel->setStyleSheet("color: orange;");
    
    // TODO: Implementar test real de API
    // Por ahora solo validamos que no esté vacía
    m_unsplashStatusLabel->setText("Configurado (no probado)");
    m_unsplashStatusLabel->setStyleSheet("color: green;");
}

void SettingsDialog::testPexelsApiKey()
{
    QString apiKey = m_pexelsApiKeyEdit->text().trimmed();
    if (apiKey.isEmpty()) {
        m_pexelsStatusLabel->setText("API key vacía");
        m_pexelsStatusLabel->setStyleSheet("color: red;");
        return;
    }
    
    m_pexelsStatusLabel->setText("Probando...");
    m_pexelsStatusLabel->setStyleSheet("color: orange;");
    
    // TODO: Implementar test real de API
    // Por ahora solo validamos que no esté vacía
    m_pexelsStatusLabel->setText("Configurado (no probado)");
    m_pexelsStatusLabel->setStyleSheet("color: green;");
}

void SettingsDialog::saveApiKeys()
{
    QSettings settings;
    
    QString unsplashKey = m_unsplashApiKeyEdit->text().trimmed();
    QString pexelsKey = m_pexelsApiKeyEdit->text().trimmed();
    
    settings.setValue("apiKeys/unsplash", unsplashKey);
    settings.setValue("apiKeys/pexels", pexelsKey);
    
    // Actualizar estados
    if (unsplashKey.isEmpty()) {
        m_unsplashStatusLabel->setText("No configurado");
        m_unsplashStatusLabel->setStyleSheet("color: #888;");
    } else {
        m_unsplashStatusLabel->setText("Configurado");
        m_unsplashStatusLabel->setStyleSheet("color: green;");
    }
    
    if (pexelsKey.isEmpty()) {
        m_pexelsStatusLabel->setText("No configurado");
        m_pexelsStatusLabel->setStyleSheet("color: #888;");
    } else {
        m_pexelsStatusLabel->setText("Configurado");
        m_pexelsStatusLabel->setStyleSheet("color: green;");
    }
}

void SettingsDialog::loadApiKeys()
{
    QSettings settings;
    
    QString unsplashKey = settings.value("apiKeys/unsplash", "").toString();
    QString pexelsKey = settings.value("apiKeys/pexels", "").toString();
    
    m_unsplashApiKeyEdit->setText(unsplashKey);
    m_pexelsApiKeyEdit->setText(pexelsKey);
    
    // Actualizar estados iniciales
    saveApiKeys();
}

QColor SettingsDialog::getCanvasBackgroundColor() const
{
    QSettings settings;
    return settings.value("appearance/canvasBackground", QColor(Qt::white)).value<QColor>();
}

QString SettingsDialog::getDefaultText() const
{
    QSettings settings;
    return settings.value("appearance/defaultText", "Texto de ejemplo").toString();
}

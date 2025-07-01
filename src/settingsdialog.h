#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QColorDialog>
#include <QFontDialog>
#include <QLineEdit>

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    
    // Getters para configuraciones
    QColor getCanvasBackgroundColor() const;
    QString getDefaultText() const;

private slots:
    void applySettings();
    void resetSettings();
    void chooseDefaultTextColor();
    void chooseDefaultFont();
    void chooseCanvasBackgroundColor();
    void testUnsplashApiKey();
    void testPexelsApiKey();
    void saveApiKeys();

private:
    void setupGeneralTab();
    void setupAppearanceTab();
    void setupExportTab();
    void setupApiKeysTab();
    void loadSettings();
    void saveSettings();
    void loadApiKeys();

    QTabWidget *m_tabWidget;
    
    // Pestaña General
    QWidget *m_generalTab;
    QCheckBox *m_startMinimizedCheck;
    QCheckBox *m_showTrayIconCheck;
    QCheckBox *m_autoSaveCheck;
    QSpinBox *m_autoSaveIntervalSpin;
    
    // Pestaña Apariencia
    QWidget *m_appearanceTab;
    QComboBox *m_themeCombo;
    QColor m_defaultTextColor;
    QFont m_defaultFont;
    QColor m_canvasBackgroundColor;
    QString m_defaultText;
    QPushButton *m_textColorBtn;
    QPushButton *m_fontBtn;
    QPushButton *m_canvasColorBtn;
    QLineEdit *m_defaultTextEdit;
    QSlider *m_uiScaleSlider;
    
    // Pestaña Exportación
    QWidget *m_exportTab;
    QComboBox *m_defaultFormatCombo;
    QSlider *m_jpegQualitySlider;
    QCheckBox *m_addWatermarkCheck;
    QComboBox *m_watermarkPositionCombo;
    
    // Pestaña API Keys
    QWidget *m_apiKeysTab;
    QLineEdit *m_unsplashApiKeyEdit;
    QLineEdit *m_pexelsApiKeyEdit;
    QPushButton *m_testUnsplashBtn;
    QPushButton *m_testPexelsBtn;
    QLabel *m_unsplashStatusLabel;
    QLabel *m_pexelsStatusLabel;
    
    // Botones
    QPushButton *m_applyBtn;
    QPushButton *m_cancelBtn;
    QPushButton *m_resetBtn;
};

#endif // SETTINGSDIALOG_H

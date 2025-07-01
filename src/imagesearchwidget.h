#ifndef IMAGESEARCHWIDGET_H
#define IMAGESEARCHWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPixmap>
#include <QTimer>
#include <QEnterEvent>
#include <QComboBox>
#include "imagedownloader.h"
#include "imageprovider.h"

class ImageResultWidget;

class ImageSearchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageSearchWidget(QWidget *parent = nullptr);

signals:
    void imageSelected(const QString &imagePath);

private slots:
    void onSearchClicked();
    void onSearchTextChanged();
    void onSearchCompleted(const QList<ImageSearchResult> &results);
    void onSearchError(const QString &error);
    void onImageDownloaded(const QString &filePath);
    void onImageDownloadError(const QString &error);
    void onImageResultClicked(const ImageSearchResult &result);
    void onPreviewReady(const QPixmap &preview, const QString &imageUrl);
    void onProviderChanged();

private:
    void setupUI();
    void clearResults();
    void showLoading(bool loading);
    void createResultWidget(const ImageSearchResult &result);
    void setupProviders();
    void updateInfoLabel();
    
    // UI Components
    QLineEdit *m_searchLineEdit;
    QComboBox *m_providerComboBox;
    QPushButton *m_searchButton;
    QScrollArea *m_resultsScrollArea;
    QWidget *m_resultsWidget;
    QGridLayout *m_resultsLayout;
    QLabel *m_statusLabel;
    QLabel *m_infoLabel;
    QProgressBar *m_progressBar;
    
    // Logic
    ImageDownloader *m_downloader;
    QTimer *m_searchTimer;
    QList<ImageSearchResult> *m_currentResults;
    QHash<QString, ImageResultWidget*> m_resultWidgets;
    QList<ImageProvider*> m_providers;
    ImageProvider *m_currentProvider;
    
    static const int RESULTS_PER_ROW = 3;
};

class ImageResultWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageResultWidget(const ImageSearchResult &result, QWidget *parent = nullptr);
    
    void setPreviewPixmap(const QPixmap &pixmap);
    const ImageSearchResult& getResult() const { return m_result; }

signals:
    void clicked(const ImageSearchResult &result);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    ImageSearchResult m_result;
    QLabel *m_previewLabel;
    QLabel *m_infoLabel;
    bool m_hovered;
    
    void updateStyle();
};

#endif // IMAGESEARCHWIDGET_H

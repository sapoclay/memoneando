#ifndef IMAGEDOWNLOADER_H
#define IMAGEDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPixmap>
#include <QProgressDialog>
#include "imageprovider.h"  // Para usar ImageSearchResult

class ImageDownloader : public QObject
{
    Q_OBJECT

public:
    explicit ImageDownloader(QObject *parent = nullptr);
    
    // Descargar imagen específica
    void downloadImage(const QString &imageUrl, const QString &fileName = QString());
    
    // Obtener vista previa de imagen
    void getImagePreview(const QString &previewUrl);

signals:
    void imageDownloaded(const QString &filePath);
    void imageDownloadError(const QString &error);
    void previewReady(const QPixmap &preview, const QString &imageUrl);
    void downloadProgress(int bytesReceived, int bytesTotal);

private slots:
    void onImageDownloaded();
    void onPreviewDownloaded();
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    QNetworkAccessManager *m_networkManager;
    
    QString generateTempFileName(const QString &originalUrl);
};

#endif // IMAGEDOWNLOADER_H

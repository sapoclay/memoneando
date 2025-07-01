#include "imagedownloader.h"
#include <QUrlQuery>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QCryptographicHash>
#include <QColorSpace>
#include <QImageReader>

ImageDownloader::ImageDownloader(QObject *parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
{
}

void ImageDownloader::downloadImage(const QString &imageUrl, const QString &fileName)
{
    if (imageUrl.isEmpty()) {
        emit imageDownloadError("URL de imagen vacía");
        return;
    }
    
    QNetworkRequest request(imageUrl);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Memoneando/1.0.1");
    
    QNetworkReply *reply = m_networkManager->get(request);
    
    // Almacenar el nombre de archivo deseado como propiedad del reply
    if (!fileName.isEmpty()) {
        reply->setProperty("fileName", fileName);
    } else {
        reply->setProperty("fileName", generateTempFileName(imageUrl));
    }
    
    connect(reply, &QNetworkReply::finished, this, &ImageDownloader::onImageDownloaded);
    connect(reply, &QNetworkReply::downloadProgress, this, &ImageDownloader::onDownloadProgress);
}

void ImageDownloader::getImagePreview(const QString &previewUrl)
{
    if (previewUrl.isEmpty()) {
        return;
    }
    
    QNetworkRequest request(previewUrl);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Memoneando/1.0.1");
    
    QNetworkReply *reply = m_networkManager->get(request);
    reply->setProperty("previewUrl", previewUrl);
    connect(reply, &QNetworkReply::finished, this, &ImageDownloader::onPreviewDownloaded);
}

void ImageDownloader::onImageDownloaded()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;
    
    reply->deleteLater();
    
    if (reply->error() != QNetworkReply::NoError) {
        emit imageDownloadError(QString("Error descargando imagen: %1").arg(reply->errorString()));
        return;
    }
    
    QString fileName = reply->property("fileName").toString();
    
    // Crear directorio de descarga temporal si no existe
    QString downloadPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/memoneando_images";
    QDir().mkpath(downloadPath);
    
    QString fullPath = downloadPath + "/" + fileName;
    
    QFile file(fullPath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(reply->readAll());
        file.close();
        emit imageDownloaded(fullPath);
    } else {
        emit imageDownloadError(QString("No se pudo guardar la imagen: %1").arg(file.errorString()));
    }
}

void ImageDownloader::onPreviewDownloaded()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;
    
    QString previewUrl = reply->property("previewUrl").toString();
    reply->deleteLater();
    
    if (reply->error() != QNetworkReply::NoError) {
        return; // Fallar silenciosamente para previews
    }
    
    QByteArray data = reply->readAll();
    
    // Crear imagen de forma segura, similar a loadImageSafely
    QImage image;
    if (!image.loadFromData(data)) {
        qWarning() << "No se pudo cargar preview desde datos:" << previewUrl;
        return;
    }
    
    // Convertir a formato estándar para evitar problemas con ICC profiles
    QImage cleanImage;
    if (image.hasAlphaChannel()) {
        cleanImage = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    } else {
        cleanImage = image.convertToFormat(QImage::Format_RGB32);
    }
    
    // Limpiar metadatos y perfiles de color
    cleanImage.setColorSpace(QColorSpace());
    
    QPixmap preview = QPixmap::fromImage(cleanImage);
    
    if (!preview.isNull()) {
        emit previewReady(preview, previewUrl);
    } else {
        qWarning() << "No se pudo convertir preview a pixmap:" << previewUrl;
    }
}

void ImageDownloader::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesTotal > 0) {
        emit downloadProgress(static_cast<int>(bytesReceived), static_cast<int>(bytesTotal));
    }
}

QString ImageDownloader::generateTempFileName(const QString &originalUrl)
{
    // Generar nombre único basado en la URL
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(originalUrl.toUtf8());
    QString hashString = hash.result().toHex();
    
    // Intentar extraer extensión de la URL
    QFileInfo fileInfo(originalUrl);
    QString extension = fileInfo.suffix();
    if (extension.isEmpty()) {
        extension = "jpg"; // Fallback
    }
    
    return QString("image_%1.%2").arg(hashString.left(12), extension);
}

#include "lorempicsumprovider.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QDebug>
#include <QRandomGenerator>

LoremPicsumProvider::LoremPicsumProvider(QObject *parent)
    : ImageProvider(parent)
    , m_baseUrl("https://picsum.photos/v2/list")
{
}

void LoremPicsumProvider::searchImages(const QString &query, int page, int perPage)
{
    // Lorem Picsum no tiene búsqueda real, pero podemos obtener una lista
    // de imágenes y filtrar o generar resultados basados en la consulta
    
    QUrl url(m_baseUrl);
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("page", QString::number(page));
    urlQuery.addQueryItem("limit", QString::number(qMin(perPage, 100))); // Max 100
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Memoneando/1.0");

    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &LoremPicsumProvider::onImageListFinished);
    
    // Guardar la consulta para usarla en el procesamiento
    reply->setProperty("query", query);
    reply->setProperty("perPage", perPage);
}

void LoremPicsumProvider::onImageListFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) {
        emit searchError("Error interno de red");
        return;
    }

    QString query = reply->property("query").toString();
    int perPage = reply->property("perPage").toInt();
    
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        // Si falla la API, generar imágenes placeholder
        generatePlaceholderResults(query, perPage);
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    
    if (!doc.isArray()) {
        generatePlaceholderResults(query, perPage);
        return;
    }

    QJsonArray imageArray = doc.array();
    QList<ImageSearchResult> results;
    
    // Mezclar los resultados para variedad
    QList<QJsonValue> shuffledImages;
    for (const QJsonValue &value : imageArray) {
        shuffledImages.append(value);
    }
    
    for (int i = 0; i < qMin(perPage, shuffledImages.size()); ++i) {
        QJsonObject imageObj = shuffledImages[i].toObject();
        
        ImageSearchResult result;
        result.id = imageObj["id"].toString();
        result.webUrl = imageObj["url"].toString();
        result.provider = "Lorem Picsum";
        result.user = imageObj["author"].toString();
        result.width = imageObj["width"].toInt();
        result.height = imageObj["height"].toInt();
        result.views = 0;
        result.downloads = 0;
        
        // Generar URLs para diferentes tamaños
        QString imageId = result.id;
        result.previewUrl = QString("https://picsum.photos/id/%1/400/300").arg(imageId);
        result.fullImageUrl = QString("https://picsum.photos/id/%1/800/600").arg(imageId);
        
        // Generar tags basados en la consulta
        if (!query.trimmed().isEmpty()) {
            result.tags = QString("placeholder, %1, fotografía").arg(query.trimmed());
        } else {
            result.tags = "placeholder, fotografía, imagen";
        }
        
        results.append(result);
    }
    
    if (results.isEmpty()) {
        generatePlaceholderResults(query, perPage);
        return;
    }
    
    emit searchCompleted(results);
}

void LoremPicsumProvider::generatePlaceholderResults(const QString &query, int perPage)
{
    QList<ImageSearchResult> results;
    
    // Generar imágenes placeholder cuando la API no está disponible
    for (int i = 0; i < perPage; ++i) {
        ImageSearchResult result;
        
        // Generar un ID aleatorio
        int randomId = QRandomGenerator::global()->bounded(1, 1000);
        result.id = QString::number(randomId);
        result.webUrl = QString("https://picsum.photos/id/%1/info").arg(randomId);
        result.provider = "Lorem Picsum";
        result.user = "Lorem Picsum";
        
        // Dimensiones variadas
        QList<QPair<int, int>> sizes = {{800, 600}, {640, 480}, {1024, 768}, {600, 400}};
        QPair<int, int> size = sizes[i % sizes.size()];
        result.width = size.first;
        result.height = size.second;
        
        result.views = 0;
        result.downloads = 0;
        
        // URLs de placeholder
        result.previewUrl = QString("https://picsum.photos/%1/%2?random=%3").arg(400).arg(300).arg(randomId);
        result.fullImageUrl = QString("https://picsum.photos/%1/%2?random=%3").arg(size.first).arg(size.second).arg(randomId);
        
        // Tags basados en la consulta
        if (!query.trimmed().isEmpty()) {
            result.tags = QString("placeholder, %1, fotografía, demo").arg(query.trimmed());
        } else {
            result.tags = "placeholder, fotografía, imagen, demo";
        }
        
        results.append(result);
    }
    
    emit searchCompleted(results);
}

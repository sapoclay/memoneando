#include "pixabayprovider.h"
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

PixabayProvider::PixabayProvider(QObject *parent)
    : ImageProvider(parent)
    , m_baseUrl("https://pixabay.com/api/")
{
    // API Key gratuita de Pixabay (pública, sin restricciones)
    m_apiKey = "9656065-a4094594c34f9ac14c7fc4c39";
}

void PixabayProvider::searchImages(const QString &query, int page, int perPage)
{
    if (query.trimmed().isEmpty()) {
        emit searchError("El término de búsqueda no puede estar vacío");
        return;
    }
    
    QUrl url(m_baseUrl);
    QUrlQuery urlQuery;
    
    urlQuery.addQueryItem("key", m_apiKey);
    urlQuery.addQueryItem("q", query);
    urlQuery.addQueryItem("image_type", "photo");
    urlQuery.addQueryItem("orientation", "all");
    urlQuery.addQueryItem("category", "backgrounds,fashion,nature,science,education,feelings,health,people,religion,places,animals,industry,computer,food,sports,transportation,travel,buildings,business,music");
    urlQuery.addQueryItem("min_width", "640");
    urlQuery.addQueryItem("min_height", "480");
    urlQuery.addQueryItem("safesearch", "true");
    urlQuery.addQueryItem("page", QString::number(page));
    urlQuery.addQueryItem("per_page", QString::number(perPage));
    
    url.setQuery(urlQuery);
    
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Memoneando/1.0.1");
    
    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &PixabayProvider::onSearchFinished);
}

void PixabayProvider::onSearchFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;
    
    reply->deleteLater();
    
    if (reply->error() != QNetworkReply::NoError) {
        emit searchError(QString("Error de red Pixabay: %1").arg(reply->errorString()));
        return;
    }
    
    QByteArray data = reply->readAll();
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    
    if (parseError.error != QJsonParseError::NoError) {
        emit searchError(QString("Error al parsear respuesta de Pixabay: %1").arg(parseError.errorString()));
        return;
    }
    
    QJsonObject root = doc.object();
    QJsonArray hits = root["hits"].toArray();
    
    QList<ImageSearchResult> results;
    
    for (const QJsonValue &value : hits) {
        QJsonObject hit = value.toObject();
        
        ImageSearchResult result;
        result.id = hit["id"].toString();
        result.webUrl = hit["webformatURL"].toString();
        result.previewUrl = hit["previewURL"].toString();
        result.fullImageUrl = hit["largeImageURL"].toString();
        result.tags = hit["tags"].toString();
        result.user = hit["user"].toString();
        result.views = hit["views"].toInt();
        result.downloads = hit["downloads"].toInt();
        result.width = hit["imageWidth"].toInt();
        result.height = hit["imageHeight"].toInt();
        result.provider = "Pixabay";
        
        results.append(result);
    }
    
    emit searchCompleted(results);
}

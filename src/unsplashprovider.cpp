#include "unsplashprovider.h"
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QSettings>

UnsplashProvider::UnsplashProvider(QObject *parent)
    : ImageProvider(parent)
    , m_baseUrl("https://api.unsplash.com/search/photos")
{
    loadApiKey();
}

void UnsplashProvider::loadApiKey()
{
    QSettings settings;
    QString apiKey = settings.value("apiKeys/unsplash", "").toString();
    
    if (!apiKey.isEmpty()) {
        m_accessKey = "Client-ID " + apiKey;
    } else {
        m_accessKey = ""; // Sin API key
    }
}

bool UnsplashProvider::isAvailable() const
{
    QSettings settings;
    QString apiKey = settings.value("apiKeys/unsplash", "").toString();
    return !apiKey.isEmpty();
}

void UnsplashProvider::searchImages(const QString &query, int page, int perPage)
{
    // Recargar API key en caso de que haya cambiado
    loadApiKey();
    
    if (m_accessKey.isEmpty()) {
        emit searchError("API key de Unsplash no configurada. Ve a Configuración → API Keys para configurarla.");
        return;
    }
    
    if (query.trimmed().isEmpty()) {
        emit searchError("El término de búsqueda no puede estar vacío");
        return;
    }
    
    QUrl url(m_baseUrl);
    QUrlQuery urlQuery;
    
    urlQuery.addQueryItem("query", query);
    urlQuery.addQueryItem("page", QString::number(page));
    urlQuery.addQueryItem("per_page", QString::number(qMin(perPage, 30))); // Máximo 30 por página
    urlQuery.addQueryItem("orientation", "landscape");
    urlQuery.addQueryItem("content_filter", "high");
    
    url.setQuery(urlQuery);
    
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Memoneando/1.0.1");
    request.setRawHeader("Authorization", m_accessKey.toUtf8());
    
    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &UnsplashProvider::onSearchFinished);
}

void UnsplashProvider::onSearchFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;
    
    reply->deleteLater();
    
    if (reply->error() != QNetworkReply::NoError) {
        if (reply->error() == QNetworkReply::AuthenticationRequiredError) {
            emit searchError("Unsplash requiere configuración de API key");
        } else {
            emit searchError(QString("Error de red Unsplash: %1").arg(reply->errorString()));
        }
        return;
    }
    
    QByteArray data = reply->readAll();
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    
    if (parseError.error != QJsonParseError::NoError) {
        emit searchError(QString("Error al parsear respuesta de Unsplash: %1").arg(parseError.errorString()));
        return;
    }
    
    QJsonObject root = doc.object();
    QJsonArray results = root["results"].toArray();
    
    QList<ImageSearchResult> imageResults;
    
    for (const QJsonValue &value : results) {
        QJsonObject photo = value.toObject();
        QJsonObject urls = photo["urls"].toObject();
        QJsonObject user = photo["user"].toObject();
        
        ImageSearchResult result;
        result.id = photo["id"].toString();
        result.webUrl = urls["regular"].toString();
        result.previewUrl = urls["thumb"].toString();
        result.fullImageUrl = urls["full"].toString();
        result.tags = photo["alt_description"].toString();
        result.user = user["name"].toString();
        result.views = 0; // Unsplash no proporciona views en la API de búsqueda
        result.downloads = 0; // Unsplash no proporciona downloads en la API de búsqueda
        result.width = photo["width"].toInt();
        result.height = photo["height"].toInt();
        result.provider = "Unsplash";
        
        imageResults.append(result);
    }
    
    emit searchCompleted(imageResults);
}

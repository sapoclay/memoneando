#include "pexelsprovider.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QSettings>
#include <QDebug>

PexelsProvider::PexelsProvider(QObject *parent)
    : ImageProvider(parent)
    , m_baseUrl("https://api.pexels.com/v1/search")
{
    loadApiKey();
}

void PexelsProvider::loadApiKey()
{
    QSettings settings;
    m_apiKey = settings.value("apiKeys/pexels", "").toString();
}

bool PexelsProvider::isAvailable() const
{
    QSettings settings;
    QString apiKey = settings.value("apiKeys/pexels", "").toString();
    return !apiKey.isEmpty();
}

bool PexelsProvider::checkApiKey() const
{
    return !m_apiKey.isEmpty() && m_apiKey.length() > 10;
}

void PexelsProvider::searchImages(const QString &query, int page, int perPage)
{
    // Recargar API key en caso de que haya cambiado
    loadApiKey();
    
    if (!checkApiKey()) {
        emit searchError("API key de Pexels no configurada. Ve a Configuración → API Keys para configurarla.");
        return;
    }

    if (query.trimmed().isEmpty()) {
        emit searchError("La consulta de búsqueda no puede estar vacía");
        return;
    }

    QUrl url(m_baseUrl);
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("query", query.trimmed());
    urlQuery.addQueryItem("page", QString::number(page));
    urlQuery.addQueryItem("per_page", QString::number(qMin(perPage, 80))); // Pexels max es 80
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Memoneando/1.0");
    request.setRawHeader("Authorization", m_apiKey.toUtf8());

    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &PexelsProvider::onSearchFinished);
}

void PexelsProvider::onSearchFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) {
        emit searchError("Error interno de red");
        return;
    }

    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        QString errorMsg = QString("Error de Pexels: %1").arg(reply->errorString());
        if (reply->error() == QNetworkReply::AuthenticationRequiredError) {
            errorMsg = "API key de Pexels inválida o expirada";
        }
        emit searchError(errorMsg);
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    
    if (!doc.isObject()) {
        emit searchError("Respuesta inválida de Pexels");
        return;
    }

    QJsonObject rootObj = doc.object();
    QJsonArray photosArray = rootObj["photos"].toArray();
    
    QList<ImageSearchResult> results;
    
    for (const QJsonValue &value : photosArray) {
        QJsonObject photo = value.toObject();
        
        ImageSearchResult result;
        result.id = QString::number(photo["id"].toInt());
        result.webUrl = photo["url"].toString();
        result.provider = "Pexels";
        result.user = photo["photographer"].toString();
        result.width = photo["width"].toInt();
        result.height = photo["height"].toInt();
        result.views = 0; // Pexels no proporciona esta información
        result.downloads = 0; // Pexels no proporciona esta información
        
        // Obtener URLs de imágenes
        QJsonObject srcObj = photo["src"].toObject();
        result.previewUrl = srcObj["medium"].toString();
        result.fullImageUrl = srcObj["large"].toString();
        
        // Pexels no proporciona tags directamente, usar alt text si está disponible
        result.tags = photo["alt"].toString();
        if (result.tags.isEmpty()) {
            result.tags = "imagen, fotografía";
        }
        
        results.append(result);
    }
    
    emit searchCompleted(results);
}

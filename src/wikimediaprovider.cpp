#include "wikimediaprovider.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QDebug>

WikimediaProvider::WikimediaProvider(QObject *parent)
    : ImageProvider(parent)
    , m_baseUrl("https://commons.wikimedia.org/w/api.php")
{
}

void WikimediaProvider::searchImages(const QString &query, int page, int perPage)
{
    if (query.trimmed().isEmpty()) {
        emit searchError("La consulta de búsqueda no puede estar vacía");
        return;
    }

    QUrl url(m_baseUrl);
    QUrlQuery urlQuery;
    
    // Usar la API de Wikimedia Commons para buscar archivos
    urlQuery.addQueryItem("action", "query");
    urlQuery.addQueryItem("format", "json");
    urlQuery.addQueryItem("list", "search");
    urlQuery.addQueryItem("srsearch", "filetype:bitmap " + query.trimmed());
    urlQuery.addQueryItem("srnamespace", "6"); // Namespace para archivos
    urlQuery.addQueryItem("srlimit", QString::number(qMin(perPage, 50))); // Max 50
    urlQuery.addQueryItem("sroffset", QString::number((page - 1) * perPage));
    urlQuery.addQueryItem("srprop", "size|wordcount|timestamp|snippet");
    
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Memoneando/1.0 (https://github.com/tu-usuario/memoneando)");

    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &WikimediaProvider::onSearchFinished);
}

void WikimediaProvider::onSearchFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) {
        emit searchError("Error interno de red");
        return;
    }

    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        emit searchError(QString("Error de Wikimedia: %1").arg(reply->errorString()));
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    
    if (!doc.isObject()) {
        emit searchError("Respuesta inválida de Wikimedia Commons");
        return;
    }

    QJsonObject rootObj = doc.object();
    QJsonObject queryObj = rootObj["query"].toObject();
    QJsonArray searchArray = queryObj["search"].toArray();
    
    QList<ImageSearchResult> results;
    
    for (const QJsonValue &value : searchArray) {
        QJsonObject item = value.toObject();
        
        QString title = item["title"].toString();
        if (!title.startsWith("File:")) {
            continue; // Solo procesar archivos
        }
        
        // Extraer el nombre del archivo sin el prefijo "File:"
        QString filename = title.mid(5);
        
        // Verificar que sea una imagen
        QString lowerFilename = filename.toLower();
        if (!lowerFilename.endsWith(".jpg") && !lowerFilename.endsWith(".jpeg") && 
            !lowerFilename.endsWith(".png") && !lowerFilename.endsWith(".gif") &&
            !lowerFilename.endsWith(".webp") && !lowerFilename.endsWith(".svg")) {
            continue;
        }
        
        ImageSearchResult result;
        result.id = QString::number(item["pageid"].toInt());
        result.webUrl = QString("https://commons.wikimedia.org/wiki/%1").arg(title);
        result.provider = "Wikimedia Commons";
        result.user = "Wikimedia Commons";
        result.width = 0; // Se determinará al descargar
        result.height = 0;
        result.views = 0;
        result.downloads = 0;
        
        // Generar URLs para la imagen
        // Wikimedia usa un sistema de thumbnails basado en el nombre del archivo
        QString encodedFilename = QString(filename).replace(" ", "_");
        QString baseImageUrl = QString("https://commons.wikimedia.org/wiki/Special:FilePath/%1").arg(encodedFilename);
        
        result.previewUrl = baseImageUrl + "?width=400";
        result.fullImageUrl = baseImageUrl;
        
        // Usar el snippet como tags si está disponible
        result.tags = item["snippet"].toString();
        if (result.tags.isEmpty()) {
            result.tags = "wikimedia, libre, dominio público";
        }
        
        results.append(result);
    }
    
    if (results.isEmpty()) {
        emit searchError("No se encontraron imágenes en Wikimedia Commons para esta búsqueda");
        return;
    }
    
    emit searchCompleted(results);
}

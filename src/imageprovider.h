#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QNetworkAccessManager>

struct ImageSearchResult {
    QString id;
    QString webUrl;
    QString previewUrl;
    QString fullImageUrl;
    QString tags;
    QString user;
    int views;
    int downloads;
    int width;
    int height;
    QString provider; // Nuevo: identificar el proveedor
};

class ImageProvider : public QObject
{
    Q_OBJECT

public:
    explicit ImageProvider(QObject *parent = nullptr);
    virtual ~ImageProvider() = default;

    // Métodos virtuales puros que cada proveedor debe implementar
    virtual QString getName() const = 0;
    virtual QString getDisplayName() const = 0;
    virtual void searchImages(const QString &query, int page = 1, int perPage = 20) = 0;
    virtual bool isAvailable() const = 0;

signals:
    void searchCompleted(const QList<ImageSearchResult> &results);
    void searchError(const QString &error);

protected:
    QNetworkAccessManager *m_networkManager;
};

#endif // IMAGEPROVIDER_H

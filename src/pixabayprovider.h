#ifndef PIXABAYPROVIDER_H
#define PIXABAYPROVIDER_H

#include "imageprovider.h"
#include <QNetworkReply>

class PixabayProvider : public ImageProvider
{
    Q_OBJECT

public:
    explicit PixabayProvider(QObject *parent = nullptr);

    QString getName() const override { return "pixabay"; }
    QString getDisplayName() const override { return "Pixabay"; }
    void searchImages(const QString &query, int page = 1, int perPage = 20) override;
    bool isAvailable() const override { return true; }

private slots:
    void onSearchFinished();

private:
    QString m_baseUrl;
    QString m_apiKey;
};

#endif // PIXABAYPROVIDER_H

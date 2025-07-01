#ifndef UNSPLASHPROVIDER_H
#define UNSPLASHPROVIDER_H

#include "imageprovider.h"
#include <QNetworkReply>

class UnsplashProvider : public ImageProvider
{
    Q_OBJECT

public:
    explicit UnsplashProvider(QObject *parent = nullptr);

    QString getName() const override { return "unsplash"; }
    QString getDisplayName() const override { return "Unsplash"; }
    void searchImages(const QString &query, int page = 1, int perPage = 20) override;
    bool isAvailable() const override;

private slots:
    void onSearchFinished();

private:
    void loadApiKey();
    QString m_baseUrl;
    QString m_accessKey;
};

#endif // UNSPLASHPROVIDER_H

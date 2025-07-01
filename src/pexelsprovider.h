#ifndef PEXELSPROVIDER_H
#define PEXELSPROVIDER_H

#include "imageprovider.h"
#include <QNetworkReply>

class PexelsProvider : public ImageProvider
{
    Q_OBJECT

public:
    explicit PexelsProvider(QObject *parent = nullptr);

    QString getName() const override { return "pexels"; }
    QString getDisplayName() const override { return "Pexels"; }
    void searchImages(const QString &query, int page = 1, int perPage = 20) override;
    bool isAvailable() const override;

private slots:
    void onSearchFinished();

private:
    void loadApiKey();
    QString m_baseUrl;
    QString m_apiKey;
    bool checkApiKey() const;
};

#endif // PEXELSPROVIDER_H

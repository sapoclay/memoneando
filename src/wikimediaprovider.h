#ifndef WIKIMEDIAPROVIDER_H
#define WIKIMEDIAPROVIDER_H

#include "imageprovider.h"
#include <QNetworkReply>

class WikimediaProvider : public ImageProvider
{
    Q_OBJECT

public:
    explicit WikimediaProvider(QObject *parent = nullptr);

    QString getName() const override { return "wikimedia"; }
    QString getDisplayName() const override { return "Wikimedia Commons"; }
    void searchImages(const QString &query, int page = 1, int perPage = 20) override;
    bool isAvailable() const override { return true; }

private slots:
    void onSearchFinished();

private:
    QString m_baseUrl;
};

#endif // WIKIMEDIAPROVIDER_H

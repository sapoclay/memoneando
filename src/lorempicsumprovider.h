#ifndef LOREMPICSUMPROVIDER_H
#define LOREMPICSUMPROVIDER_H

#include "imageprovider.h"
#include <QNetworkReply>

class LoremPicsumProvider : public ImageProvider
{
    Q_OBJECT

public:
    explicit LoremPicsumProvider(QObject *parent = nullptr);

    QString getName() const override { return "lorempicsum"; }
    QString getDisplayName() const override { return "Lorem Picsum"; }
    void searchImages(const QString &query, int page = 1, int perPage = 20) override;
    bool isAvailable() const override { return true; }

private slots:
    void onImageListFinished();

private:
    QString m_baseUrl;
    void generatePlaceholderResults(const QString &query, int perPage);
};

#endif // LOREMPICSUMPROVIDER_H

#include "imageprovider.h"
#include <QNetworkAccessManager>

ImageProvider::ImageProvider(QObject *parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
{
}

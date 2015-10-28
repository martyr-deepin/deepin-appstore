#include <QDebug>
#include "../Shell.h"
#include "NetworkAccessManager.h"
#include "CookieJar.h"

NetworkAccessManager::NetworkAccessManager(QObject *parent) : QNetworkAccessManager(parent) {
    auto shell = static_cast<Shell*>(qApp);
    auto cookieJar = new CookieJar(this);
    this->setCookieJar(cookieJar);

    this->diskCache = new QNetworkDiskCache(this);
    this->diskCache->setCacheDirectory(shell->basePath + "/cache");
    this->setCache(this->diskCache);
}

NetworkAccessManager::~NetworkAccessManager() {

}

#include <QDebug>
#include "../Shell.h"
#include "NetworkAccessManager.h"
#include "CookieJar.h"
#include "LocalFileSystemReply.h"

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

QNetworkReply *NetworkAccessManager::createRequest(QNetworkAccessManager::Operation op,
                                                   const QNetworkRequest &req,
                                                   QIODevice *outgoingData) {
    const auto shell = static_cast<Shell*>(qApp);
    if (shell->isInitialRun && NetworkAccessManager::isLocallyServable(req.url())) {
        qDebug() << "Loading local copy of" << req.url();
        return new LocalFileSystemReply(op, req, this, outgoingData);
    } else {
        return QNetworkAccessManager::createRequest(op, req, outgoingData);
    }

}

bool NetworkAccessManager::isLocallyServable(const QUrl& url) {
    const auto host = url.host();
    if (host == "appstore.deepin.test" ||
        host == "preview.appstore.deepin.test" ||
        host == "appstore.deepin.org") {
        const auto path = url.path();
        if (!(path.contains("/endpoints/v1/") ||
              path.contains("/data/v1/"))) {
            return true;
        }
    }
    return false;
}

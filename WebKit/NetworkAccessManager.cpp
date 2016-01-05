#include <chrono>
#include <QDebug>
#include <QNetworkDiskCache>
#include <QCommandLineParser>

#include "../Shell.h"

#include "NetworkAccessManager.h"
#include "CookieJar.h"
#include "LocalFileSystemReply.h"

#ifdef DEBUG_LOCAL_REQUEST
auto debugLocalRequest() -> QDebug {
    return qDebug() << "[LOCALREQ]" << std::chrono::system_clock::now().time_since_epoch().count();
}
#endif

NetworkAccessManager::NetworkAccessManager(QObject *parent) : QNetworkAccessManager(parent) {
    const auto shell = static_cast<Shell*>(qApp);
    const auto cookieJar = new CookieJar(this);
    this->setCookieJar(cookieJar);

    this->diskCache = new QNetworkDiskCache(this);
    this->diskCache->setCacheDirectory(shell->basePath + "/cache");
    this->setCache(this->diskCache);
    if (shell->argsParser->isSet("offline")) {
        this->setNetworkAccessible(QNetworkAccessManager::NetworkAccessibility::NotAccessible);
    }
}

NetworkAccessManager::~NetworkAccessManager() {

}

QNetworkReply *NetworkAccessManager::createRequest(QNetworkAccessManager::Operation op,
                                                   const QNetworkRequest &req,
                                                   QIODevice *outgoingData) {
    if (NetworkAccessManager::isLocallyServable(req.url())) {
#ifdef DEBUG_LOCAL_REQUEST
        debugLocalRequest() << "Loading local" << req.url();
#endif
        return new LocalFileSystemReply(op, req, this, outgoingData);
    } else {
        return QNetworkAccessManager::createRequest(op, req, outgoingData);
    }

}

bool NetworkAccessManager::isLocallyServable(const QUrl& url) {
    const auto host = url.host();
    if (host.endsWith("appstore.deepin.test") ||
        host.endsWith("appstore.deepin.org")) {
        const auto path = url.path();
        if (!(path.contains("/endpoints/v1/") ||
              path.contains("/data/v1/"))) {
            return true;
        }
    }
    return false;
}

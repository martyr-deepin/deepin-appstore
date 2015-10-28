#include <QDebug>
#include "NetworkAccessManager.h"
#include "CookieJar.h"

NetworkAccessManager::NetworkAccessManager(QObject *parent) : QNetworkAccessManager(parent) {
    auto cookieJar = new CookieJar(this);
    this->setCookieJar(cookieJar);
}

NetworkAccessManager::~NetworkAccessManager() {

}

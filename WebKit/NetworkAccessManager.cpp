/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
#include <chrono>
#include <QDebug>
#include <QNetworkDiskCache>
#include <QCommandLineParser>
#include <QFile>

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
    return QFile("/usr/share/deepin-appstore/webapp/" + url.path()).exists();
}

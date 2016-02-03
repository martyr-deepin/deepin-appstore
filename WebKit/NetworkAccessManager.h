/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
#ifndef SHELL_NETWORKACCESSMANAGER_H
#define SHELL_NETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>
class QNetworkDiskCache;
class QNetworkRequest;

class NetworkAccessManager : public QNetworkAccessManager {
public:
    explicit NetworkAccessManager(QObject* parent = nullptr);
    ~NetworkAccessManager();

private:
    QNetworkDiskCache* diskCache = nullptr;
    QNetworkReply* createRequest(Operation op,
                                 const QNetworkRequest& req,
                                 QIODevice* outgoingData = nullptr);

    static bool isLocallyServable(const QUrl& url);
};


#endif //SHELL_NETWORKACCESSMANAGER_H

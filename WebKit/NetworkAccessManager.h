/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

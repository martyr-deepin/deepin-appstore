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

#ifndef SHELL_LOCALFILESYSTEMREPLY_H
#define SHELL_LOCALFILESYSTEMREPLY_H

#include <QNetworkReply>

class LocalFileSystemReply : public QNetworkReply {
    Q_OBJECT
public:
    explicit LocalFileSystemReply(QNetworkAccessManager::Operation op,
                                  const QNetworkRequest &req,
                                  QObject* parent = nullptr,
                                  QIODevice *outgoingData = nullptr);
    ~LocalFileSystemReply();


    void close() override;
    bool isSequential() const override;
    qint64 bytesAvailable() const override;

public Q_SLOTS:
    void abort() override;
    void ignoreSslErrors() override;

protected:
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;
    void sslConfigurationImplementation(QSslConfiguration &) const override;
    void setSslConfigurationImplementation(const QSslConfiguration &) override;
    void ignoreSslErrorsImplementation(const QList<QSslError> &) override;

private:
    QByteArray content;
    qint64 offset = 0;

    QTimer* timer = nullptr;
};


#endif //SHELL_LOCALFILESYSTEMREPLY_H

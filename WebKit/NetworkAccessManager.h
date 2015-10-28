#ifndef SHELL_NETWORKACCESSMANAGER_H
#define SHELL_NETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkDiskCache>

class NetworkAccessManager : public QNetworkAccessManager {
public:
    explicit NetworkAccessManager(QObject* parent = nullptr);
    ~NetworkAccessManager();

private:
    QNetworkDiskCache* diskCache = nullptr;
};


#endif //SHELL_NETWORKACCESSMANAGER_H

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

#ifndef SHELL_NETWORKACCESSMANAGER_H
#define SHELL_NETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>

class NetworkAccessManager : public QNetworkAccessManager {
public:
    explicit NetworkAccessManager(QObject* parent = nullptr);
    ~NetworkAccessManager();


};


#endif //SHELL_NETWORKACCESSMANAGER_H

#ifndef SHELL_COOKIEJAR_H
#define SHELL_COOKIEJAR_H

#include <QNetworkCookieJar>

class CookieJar : public QNetworkCookieJar {
public:
    explicit CookieJar(QObject* parent = nullptr);
    ~CookieJar();
};


#endif //SHELL_COOKIEJAR_H

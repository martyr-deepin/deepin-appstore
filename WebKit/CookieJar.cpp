#include <QNetworkCookie>

#include "../Shell.h"
#include "CookieJar.h"

CookieJar::CookieJar(QObject *parent) : QNetworkCookieJar(parent) {
    auto shell = static_cast<Shell*>(qApp);
    shell->settings->beginGroup("Network");
    this->setAllCookies(QNetworkCookie::parseCookies(shell->settings->value("cookies").toByteArray()));
    shell->settings->endGroup();
}

CookieJar::~CookieJar() {
    auto shell = static_cast<Shell*>(qApp);
    auto cookies = this->allCookies();
    for (int i = cookies.count() - 1; i >= 0; --i) {
        if (cookies.at(i).isSessionCookie()) {
            cookies.removeAt(i);
        }
    }

    QString cookiesStr = "";
    for (int i = 0; i < cookies.count(); i++) {
        cookiesStr += cookies.at(i).toRawForm() + "\n";
    }
    shell->settings->beginGroup("Network");
    shell->settings->setValue("cookies", cookiesStr);
    shell->settings->endGroup();
    shell->settings->sync();
}

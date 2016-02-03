/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
#include <QNetworkCookie>
#include <QSettings>

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

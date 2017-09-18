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

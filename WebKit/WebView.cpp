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

#include <QCommandLineParser>

#include "Shell.h"
#include "WebView.h"
#include "WebPage.h"
#include "NetworkAccessManager.h"

WebView::WebView(QWidget *parent) : QWebView(parent) {
    this->customPage = new WebPage(this);
    this->setPage(this->customPage);

    if (!(static_cast<Shell*>(qApp))->argsParser->isSet("disableDataCache")) {
        const auto nam = new NetworkAccessManager(this->customPage);
        this->customPage->setNetworkAccessManager(nam);
    }

    this->setAcceptDrops(false);
    this->setAttribute(Qt::WA_OpaquePaintEvent, true);

    const auto shell = static_cast<Shell*>(qApp);
    const auto settings = this->settings();
    settings->enablePersistentStorage(shell->basePath + "/storage");

    settings->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, false);
    settings->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    settings->setAttribute(QWebSettings::LocalStorageEnabled, true);

    if (shell->argsParser->isSet("debug")) {
        settings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    } else {
        this->setContextMenuPolicy(Qt::NoContextMenu);
    }
}

WebView::~WebView() {

}

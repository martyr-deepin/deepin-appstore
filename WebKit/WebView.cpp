/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
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

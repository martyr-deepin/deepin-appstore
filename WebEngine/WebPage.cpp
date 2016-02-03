/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
#include <QWebChannel>
#include <QWebEngineProfile>

#include "WebPage.h"
#include "../Bridge.h"

WebPage::WebPage(QWidget *parent) : QWebEnginePage(parent) {
    const auto webChannel = new QWebChannel(this);
    const auto bridge = new Bridge(this);
    webChannel->registerObject("bridge", bridge);
    this->setWebChannel(webChannel);

    const auto profile = this->profile();
    const auto ua = profile->httpUserAgent() + " " + qApp->applicationName() + "/" + qApp->applicationVersion();
    profile->setHttpUserAgent(ua);
}

WebPage::~WebPage() {

}

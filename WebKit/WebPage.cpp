/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
#include "common.h"

#include <QDebug>
#include <QWebFrame>
#include "WebPage.h"
#include "Bridge.h"
#include "NetworkAccessManager.h"

WebPage::WebPage(QWidget* parent) : QWebPage(parent) {
    const auto nam = new NetworkAccessManager(this);
    this->setNetworkAccessManager(nam);
    QObject::connect(this->mainFrame(), &QWebFrame::javaScriptWindowObjectCleared,
                     this, &WebPage::addBridge);
}

WebPage::~WebPage() {
    if (this->bridge) {
        delete this->bridge;
        this->bridge = nullptr;
    }
}

void WebPage::addBridge() {
    if (this->bridge) {
        delete this->bridge;
    }
    this->bridge = new Bridge(this);
    this->mainFrame()->addToJavaScriptWindowObject("bridge", this->bridge);
}

void WebPage::javaScriptConsoleMessage(const QString& message,
                                       int UNUSED(lineNumber),
                                       const QString& sourceID) {
    qDebug() << sourceID << "==> " << message;
}

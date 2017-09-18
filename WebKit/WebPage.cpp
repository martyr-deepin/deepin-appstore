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

#include "common.h"

#include <QDebug>
#include <QWebFrame>
#include "WebPage.h"
#include "Bridge.h"


WebPage::WebPage(QWidget* parent) : QWebPage(parent) {
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

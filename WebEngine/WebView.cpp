/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "WebView.h"
#include "WebPage.h"

WebView::WebView(QWidget *parent) : QWebEngineView(parent) {
    const auto customPage = new WebPage(this);
    this->setPage(customPage);
    this->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
}

WebView::~WebView() {

}

/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
#ifndef SHELL_WEBKIT_WEBVIEW_H
#define SHELL_WEBKIT_WEBVIEW_H

#include <QWebView>
class WebPage;

class WebView : public QWebView {
    Q_OBJECT

public:
    explicit WebView(QWidget* parent = nullptr);
    ~WebView();

private:
    WebPage* customPage = nullptr;
};


#endif //SHELL_WEBKIT_WEBVIEW_H

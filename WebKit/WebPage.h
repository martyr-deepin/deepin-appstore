/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef SHELL_WEBKIT_WEBPAGE_H
#define SHELL_WEBKIT_WEBPAGE_H

#include <QWebPage>

class WebPage : public QWebPage {
    Q_OBJECT
public:
    explicit WebPage(QWidget* parent = nullptr);
    ~WebPage();

    void javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& sourceID) override;
public Q_SLOTS:
    void addBridge();

private:
    QObject* bridge = nullptr;
};


#endif //SHELL_WEBKIT_WEBPAGE_H

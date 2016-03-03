/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef SHELL_MAINWINDOW_H
#define SHELL_MAINWINDOW_H



#include <QWidget>
#include "StupidWindow.h"
#include "WebWidget.h"

class QHBoxLayout;



class MainWindow : public StupidWindow {
    Q_OBJECT

public:
    explicit MainWindow(StupidWindow* parent = nullptr);
    ~MainWindow();

    void toggleMaximized();
    void setUrl(const QUrl& url);


signals:
    void windowStateChanged(Qt::WindowState state);

private:
    WebView* webView = nullptr;

protected:
    bool event(QEvent* event) override;
    void changeEvent(QEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
};

#endif //SHELL_MAINWINDOW_H

/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef SHELL_ABOUTWINDOW_H
#define SHELL_ABOUTWINDOW_H
#include <QWidget>
class WebView;
class QGraphicsDropShadowEffect;

class AboutWindow : public QWidget {
    Q_OBJECT

public:
    explicit AboutWindow(QWidget* parent = nullptr);
    ~AboutWindow();

    void setContent(const QString& html);

protected:
    void keyPressEvent(QKeyEvent * event);

private:
    WebView* content = nullptr;

    unsigned int contentWidth = 0;
    unsigned int contentHeight = 0;
};

#endif //SHELL_ABOUTWINDOW_H

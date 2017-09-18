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

#ifndef SHELL_MAINWINDOW_H
#define SHELL_MAINWINDOW_H



#include <QWidget>
#include "WebWidget.h"

class QHBoxLayout;

enum CornerEdge {
    Nil = 0,
    Top = 1,
    Right = 2,
    Bottom = 4,
    Left = 8,
    TopLeft = Top | Left,
    TopRight = Top | Right,
    BottomLeft = Bottom | Left,
    BottomRight = Bottom | Right,
};


class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void toggleMaximized();
    void setUrl(const QUrl& url);

    void updateCursor(CornerEdge);


Q_SIGNALS:
    void windowStateChanged(Qt::WindowState state);

private:
    WebView* webView = nullptr;

protected:
    bool event(QEvent* event) override;
    void changeEvent(QEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

    void setBorderRadius(int s);
};

#endif //SHELL_MAINWINDOW_H

/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
#ifndef SHELL_STUPIDWINDOW_H
#define SHELL_STUPIDWINDOW_H

/**
 * Every fucking thing about this window is stupid!
 */


#include <QWidget>
class QGraphicsDropShadowEffect;
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

class StupidWindow : public QWidget {
    Q_OBJECT

public:
    explicit StupidWindow(QWidget* parent = nullptr);
    ~StupidWindow();

    void polish();

    QPoint mapToGlobal(const QPoint &) const;
    void resize(int w, int h);
    void setFixedSize(int w, int h);
    void setMinimumSize(int w, int h);
    void setMaximumSize(int maxw, int maxh);
    void setModal(bool);

public Q_SLOTS:
    void startMoving();
    void updateCursor(CornerEdge);
    void showMaximized();
    void showNormal();
    void showMinimized();

protected:
    int shadowOffsetX = 0;
    int shadowOffsetY = 6;

    void changeEvent(QEvent* event) override;
    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent*) override;

private:
    QHBoxLayout* horizontalLayout = nullptr;
    void startResizing(const QPoint& globalPoint, const CornerEdge& ce);

    const int resizeHandleWidth = 0;
    unsigned const int shadowRadius = 0;
    const int layoutMargin = 0;
    const unsigned int borderRadius = 0;
    const QColor borderColor = QColor(0, 0, 0, 255 / 5);

    int userMinimumWidth = 0;
    int userMinimumHeight = 0;
    void applyMinimumSizeRestriction();

    int userMaximumWidth = QWIDGETSIZE_MAX;
    int userMaximumHeight = QWIDGETSIZE_MAX;
    void applyMaximumSizeRestriction();

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    QGraphicsDropShadowEffect* shadowEffect = nullptr;

    CornerEdge resizingCornerEdge = CornerEdge::Nil;
    CornerEdge getCornerEdge(int, int);

    void setMargins(unsigned int width);
    void paintOutline();
};


#endif //SHELL_STUPIDWINDOW_H

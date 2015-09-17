
#ifndef SHELL_MAINWINDOW_H
#define SHELL_MAINWINDOW_H

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

#include <QWidget>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>

#include "WebWidget.h"

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void showLessImportant();

    void startMoving(int x, int y);
    void toggleMaximized();

    void resizeContent(int w, int h);
    void setMinimumContentSize(int w, int h);

private:
    unsigned int resizeHandleWidth = 5;

    QHBoxLayout *horizontalLayout;
    WebView* webView = nullptr;
    QGraphicsDropShadowEffect* shadowEffect;

    CornerEdge resizingCornerEdge = CornerEdge::Nil;
    QRect beforeResizing;
    bool recalcResizingInProgress = false;
    QPoint beforeResizingGlobalPos;

    CornerEdge getCornerEdge(int, int);
    void updateCursor(CornerEdge);

protected:
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
};

#endif //SHELL_MAINWINDOW_H

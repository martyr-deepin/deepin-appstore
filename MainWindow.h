
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
#include "WebWidget.h"
class QHBoxLayout;
class QGraphicsDropShadowEffect;


class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void polish();

    void startMoving(int x, int y);
    void toggleMaximized();

    void resizeContent(int w, int h);
    void setMinimumContentSize(int w, int h);
    void setUrl(const QUrl& url);

signals:
    void windowStateChanged(Qt::WindowState state);

private:
    unsigned int resizeHandleWidth = 5;

    QHBoxLayout *horizontalLayout = nullptr;
    WebView* webView = nullptr;
    QGraphicsDropShadowEffect* shadowEffect = nullptr;

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
    void changeEvent(QEvent* event) Q_DECL_OVERRIDE;
};

#endif //SHELL_MAINWINDOW_H

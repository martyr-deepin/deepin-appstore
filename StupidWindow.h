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
    void setMinimumSize(int w, int h);
    void setMaximumSize(int maxw, int maxh);

public slots:
    void startMoving();
    void updateCursor(CornerEdge);

protected:
    void changeEvent(QEvent* event) Q_DECL_OVERRIDE;

private:
    QHBoxLayout* horizontalLayout = nullptr;
    void startResizing(const QPoint& globalPoint, const CornerEdge& ce);

    const int resizeHandleWidth = 0;
    unsigned const int shadowRadius = 0;
    const int layoutMargin = 0;

    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;


    bool recalcResizingInProgress = false;

    QGraphicsDropShadowEffect* shadowEffect = nullptr;

    CornerEdge resizingCornerEdge = CornerEdge::Nil;
    CornerEdge getCornerEdge(int, int);

};


#endif //SHELL_STUPIDWINDOW_H

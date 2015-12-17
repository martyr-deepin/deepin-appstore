#include <QGraphicsDropShadowEffect>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QHBoxLayout>
#include "StupidWindow.h"

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <QX11Info>

#define _NET_WM_MOVERESIZE_SIZE_TOPLEFT      0
#define _NET_WM_MOVERESIZE_SIZE_TOP          1
#define _NET_WM_MOVERESIZE_SIZE_TOPRIGHT     2
#define _NET_WM_MOVERESIZE_SIZE_RIGHT        3
#define _NET_WM_MOVERESIZE_SIZE_BOTTOMRIGHT  4
#define _NET_WM_MOVERESIZE_SIZE_BOTTOM       5
#define _NET_WM_MOVERESIZE_SIZE_BOTTOMLEFT   6
#define _NET_WM_MOVERESIZE_SIZE_LEFT         7
#define _NET_WM_MOVERESIZE_MOVE              8

int bound(int min, int between, int max) {
    if (between < min) {
        return min;
    }
    if (between > max) {
        return max;
    }
    return between;
}

auto cornerEdge2WmGravity(const CornerEdge& ce) -> int {
    switch (ce) {
        case CornerEdge::Top:
            return _NET_WM_MOVERESIZE_SIZE_TOP;
        case CornerEdge::TopRight:
            return _NET_WM_MOVERESIZE_SIZE_TOPRIGHT;
        case CornerEdge::Right:
            return _NET_WM_MOVERESIZE_SIZE_RIGHT;
        case CornerEdge::BottomRight:
            return _NET_WM_MOVERESIZE_SIZE_BOTTOMRIGHT;
        case CornerEdge::Bottom:
            return _NET_WM_MOVERESIZE_SIZE_BOTTOM;
        case CornerEdge::BottomLeft:
            return _NET_WM_MOVERESIZE_SIZE_BOTTOMLEFT;
        case CornerEdge::Left:
            return _NET_WM_MOVERESIZE_SIZE_LEFT;
        case CornerEdge::TopLeft:
            return _NET_WM_MOVERESIZE_SIZE_TOPLEFT;
        default: {}
    }
    throw "Not a resizing CornerEdge";
}


StupidWindow::StupidWindow(QWidget* parent) : QWidget(parent),
                                              resizeHandleWidth(5),
                                              shadowRadius(24),
                                              layoutMargin(25) {
#ifndef PLAIN_VISUAL_EFFECT
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setWindowFlags(Qt::FramelessWindowHint);
#endif
    this->horizontalLayout = new QHBoxLayout(this);
    this->horizontalLayout->setSpacing(0);
    this->horizontalLayout->setObjectName("horizontalLayout");
    this->horizontalLayout->setContentsMargins(this->layoutMargin,
                                               this->layoutMargin,
                                               this->layoutMargin,
                                               this->layoutMargin);
    this->setLayout(this->horizontalLayout);
}

StupidWindow::~StupidWindow() {

}

void StupidWindow::polish() {
#ifndef PLAIN_VISUAL_EFFECT
    if (!this->shadowEffect) {
        this->shadowEffect = new QGraphicsDropShadowEffect(this);
        this->shadowEffect->setBlurRadius(this->shadowRadius);
        this->shadowEffect->setColor(QColor(0, 0, 0, 255 / 5));
        this->shadowEffect->setOffset(0, 6);
        this->setGraphicsEffect(this->shadowEffect);
    }
#endif
}


void StupidWindow::mousePressEvent(QMouseEvent* event) {
    const int x = event->x();
    const int y = event->y();
    if (event->button() == Qt::LeftButton) {
        const CornerEdge ce = getCornerEdge(x, y);
        if (ce) {
            resizingCornerEdge = ce;
            qDebug() << "mouse resize starts";
            return this->startResizing(QCursor::pos(), ce);
        }
    }
    QWidget::mousePressEvent(event);
}

void StupidWindow::mouseMoveEvent(QMouseEvent* event) {
    const int x = event->x();
    const int y = event->y();

    const CornerEdge ce = getCornerEdge(x, y);
    if (!resizingCornerEdge) {
        this->updateCursor(ce);
    }
    QWidget::mouseMoveEvent(event);
}

void StupidWindow::startResizing(const QPoint& globalPoint, const CornerEdge& ce) {
    XEvent xev;
    Atom netMoveResize = XInternAtom(QX11Info::display(), "_NET_WM_MOVERESIZE", false);
    xev.xclient.type = ClientMessage;
    xev.xclient.message_type = netMoveResize;
    xev.xclient.display = QX11Info::display();

    xev.xclient.window = this->winId();
    xev.xclient.format = 32;

    xev.xclient.data.l[0] = globalPoint.x();
    xev.xclient.data.l[1] = globalPoint.y();
    xev.xclient.data.l[2] = cornerEdge2WmGravity(ce);
    xev.xclient.data.l[3] = Button1;
    xev.xclient.data.l[4] = 1;
    XUngrabPointer(QX11Info::display(), QX11Info::appTime());

    XSendEvent(QX11Info::display(),
               QX11Info::appRootWindow(QX11Info::appScreen()),
               false,
               SubstructureRedirectMask | SubstructureNotifyMask,
               &xev);
}

void StupidWindow::mouseReleaseEvent(QMouseEvent* event) {
    QWidget::mouseReleaseEvent(event);
    if (this->resizingCornerEdge) {
        this->resizingCornerEdge = CornerEdge::Nil;
    }
}

CornerEdge StupidWindow::getCornerEdge(int x, int y) {
    const QSize winSize = size();
    unsigned int ce = (unsigned int)CornerEdge::Nil;

    const auto outer = this->layoutMargin - this->resizeHandleWidth;
    const auto inner = this->layoutMargin;

    if (outer <= y && y <= inner) {
        ce = ce | (unsigned int)CornerEdge::Top;
    }
    if (outer <= x && x <= inner) {
        ce = ce | (unsigned int)CornerEdge::Left;
    }
    if (winSize.height() - inner <= y && y <= winSize.height() - outer) {
        ce = ce | (unsigned int)CornerEdge::Bottom;
    }
    if (winSize.width() - inner <= x && x <= winSize.width() - outer) {
        ce = ce | (unsigned int)CornerEdge::Right;
    }
    return (CornerEdge)ce;
}

void StupidWindow::updateCursor(CornerEdge ce) {
    switch (ce) {
        case CornerEdge::Nil: {
            this->unsetCursor();
            break;
        }
        case CornerEdge::Top:
        case CornerEdge::Bottom: {
            this->setCursor(Qt::SizeVerCursor);
            break;
        }
        case CornerEdge::Left:
        case CornerEdge::Right: {
            this->setCursor(Qt::SizeHorCursor);
            break;
        }
        case CornerEdge::TopLeft:
        case CornerEdge::BottomRight: {
            this->setCursor(Qt::SizeFDiagCursor);
            break;
        }
        case CornerEdge::TopRight:
        case CornerEdge::BottomLeft: {
            this->setCursor(Qt::SizeBDiagCursor);
            break;
        };
    }
}

void StupidWindow::startMoving() {
    XEvent xev;
    Atom netMoveResize = XInternAtom(QX11Info::display(), "_NET_WM_MOVERESIZE", false);
    xev.xclient.type = ClientMessage;
    xev.xclient.message_type = netMoveResize;
    xev.xclient.display = QX11Info::display();

    xev.xclient.window = this->winId();
    xev.xclient.format = 32;

    const auto globalPos = QCursor::pos();
    xev.xclient.data.l[0] = globalPos.x();
    xev.xclient.data.l[1] = globalPos.y();
    xev.xclient.data.l[2] = _NET_WM_MOVERESIZE_MOVE;
    xev.xclient.data.l[3] = Button1;
    xev.xclient.data.l[4] = 1;
    XUngrabPointer(QX11Info::display(), QX11Info::appTime());

    XSendEvent(QX11Info::display(),
               QX11Info::appRootWindow(QX11Info::appScreen()),
               false,
               SubstructureRedirectMask | SubstructureNotifyMask,
               &xev);
}


QPoint StupidWindow::mapToGlobal(const QPoint& point) const {
    auto result = QWidget::mapToGlobal(point);
    const auto currentLayoutMargin = this->contentsMargins().left();
    result.setX(result.x() + currentLayoutMargin);
    result.setY(result.y() + currentLayoutMargin);
    return result;
}

void StupidWindow::changeEvent(QEvent *event) {
    QWidget::changeEvent(event);
    if (event->type() == QEvent::WindowStateChange) {
        if (this->windowState() & Qt::WindowMaximized) {
            this->horizontalLayout->setContentsMargins(0, 0, 0, 0);
        } else {
            this->horizontalLayout->setContentsMargins(this->layoutMargin,
                                                       this->layoutMargin,
                                                       this->layoutMargin,
                                                       this->layoutMargin);
        }
    }
}

void StupidWindow::resize(int w, int h) {
    QWidget::resize(w + this->layoutMargin * 2,
                    h + this->layoutMargin * 2);
}

void StupidWindow::setMinimumSize(int w, int h) {
    QWidget::setMinimumSize(w + this->layoutMargin * 2,
                            h + this->layoutMargin * 2);
}

void StupidWindow::setMaximumSize(int maxw, int maxh) {
    QWidget::setMaximumSize(maxw + this->layoutMargin * 2,
                            maxh + this->layoutMargin * 2);
}

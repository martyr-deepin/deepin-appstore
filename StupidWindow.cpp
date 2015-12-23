#include <QGraphicsDropShadowEffect>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QHBoxLayout>
#include <QPainter>
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

#define _NET_WM_STATE_REMOVE        0    /* remove/unset property */
#define _NET_WM_STATE_ADD           1    /* add/set property */
#define _NET_WM_STATE_TOGGLE        2    /* toggle property  */

#define XC_top_side 138
#define XC_top_right_corner 136
#define XC_right_side 96
#define XC_bottom_right_corner 14
#define XC_bottom_side 16
#define XC_bottom_left_corner 12
#define XC_left_side 70
#define XC_top_left_corner 134

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

auto cornerEdge2XCursor(const CornerEdge& ce) -> int {
    switch (ce) {
        case CornerEdge::Top:
            return XC_top_side;
        case CornerEdge::TopRight:
            return XC_top_right_corner;
        case CornerEdge::Right:
            return XC_right_side;
        case CornerEdge::BottomRight:
            return XC_bottom_right_corner;
        case CornerEdge::Bottom:
            return XC_bottom_side;
        case CornerEdge::BottomLeft:
            return XC_bottom_left_corner;
        case CornerEdge::Left:
            return XC_left_side;
        case CornerEdge::TopLeft:
            return XC_top_left_corner;
        default: {
            return -1;
        }
    }
}

StupidWindow::StupidWindow(QWidget* parent) : QWidget(parent),
                                              resizeHandleWidth(5),
                                              shadowRadius(24),
                                              layoutMargin(25),
                                              borderRadius(4) {
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setWindowFlags(Qt::FramelessWindowHint);

    this->horizontalLayout = new QHBoxLayout(this);
    this->horizontalLayout->setSpacing(0);
    this->horizontalLayout->setObjectName("horizontalLayout");
    this->setMargins(this->layoutMargin);
    this->setLayout(this->horizontalLayout);
}

StupidWindow::~StupidWindow() {

}

void StupidWindow::polish() {
#ifndef BUILD_WITH_WEBENGINE
    // draw window shadow
    if (!this->shadowEffect) {
        this->shadowEffect = new QGraphicsDropShadowEffect(this);
        this->shadowEffect->setBlurRadius(this->shadowRadius);
        this->shadowEffect->setColor(this->borderColor);
        this->shadowEffect->setOffset(0, 6);
        this->setGraphicsEffect(this->shadowEffect);
    }
#endif

    // cut round corners
    const auto layout = this->layout();
    if (!layout ||
        layout->count() == 0) {
        return;
    }

    const auto widget = this->layout()->itemAt(0)->widget();
    if (this->isMaximized() || this->isFullScreen()) {
        widget->clearMask();
    } else {
        const auto region = QRegion(widget->rect(), QRegion::RegionType::Rectangle);

        const auto tl = QRegion(0, 0, borderRadius, borderRadius, QRegion::RegionType::Rectangle).subtracted(
                QRegion(0, 0, borderRadius * 2, borderRadius * 2, QRegion::RegionType::Ellipse)
        );
        const auto tr = QRegion(widget->width() - borderRadius, 0, borderRadius, borderRadius, QRegion::RegionType::Rectangle).subtracted(
                QRegion(widget->width() - 2 * borderRadius, 0, borderRadius * 2, borderRadius * 2, QRegion::RegionType::Ellipse)
        );
        const auto bl = QRegion(0, widget->height() - borderRadius, borderRadius, borderRadius, QRegion::RegionType::Rectangle).subtracted(
                QRegion(0, widget->height() - 2 * borderRadius, borderRadius * 2, borderRadius * 2, QRegion::RegionType::Ellipse)
        );
        const auto br = QRegion(widget->width() - borderRadius, widget->height() - borderRadius, borderRadius, borderRadius, QRegion::RegionType::Rectangle).subtracted(
                QRegion(widget->width() - 2 * borderRadius, widget->height() - 2 * borderRadius, borderRadius * 2, borderRadius * 2, QRegion::RegionType::Ellipse)
        );

        const auto result = region
                .subtracted(tl)
                .subtracted(tr)
                .subtracted(bl)
                .subtracted(br);
        widget->setMask(result);
    }
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
    const auto display = QX11Info::display();
    const auto winId = this->winId();
    const auto screen = QX11Info::appScreen();

    XEvent xev;
    const Atom netMoveResize = XInternAtom(display, "_NET_WM_MOVERESIZE", false);
    xev.xclient.type = ClientMessage;
    xev.xclient.message_type = netMoveResize;
    xev.xclient.display = display;
    xev.xclient.window = winId;
    xev.xclient.format = 32;

    xev.xclient.data.l[0] = globalPoint.x();
    xev.xclient.data.l[1] = globalPoint.y();
    xev.xclient.data.l[2] = cornerEdge2WmGravity(ce);
    xev.xclient.data.l[3] = Button1;
    xev.xclient.data.l[4] = 1;
    XUngrabPointer(display, QX11Info::appTime());

    XSendEvent(display,
               QX11Info::appRootWindow(screen),
               false,
               SubstructureRedirectMask | SubstructureNotifyMask,
               &xev);
    XFlush(display);
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
    const auto display = QX11Info::display();
    const auto winId = this->winId();

    const auto XCursor = cornerEdge2XCursor(ce);
    if (XCursor != -1) {
        const auto cursor = XCreateFontCursor(display, XCursor);
        XDefineCursor(display, winId, cursor);
    } else {
        XUndefineCursor(display, winId);
    }
    XFlush(display);
}

void StupidWindow::startMoving() {
    const auto display = QX11Info::display();
    const auto winId = this->winId();
    const auto screen = QX11Info::appScreen();

    XEvent xev;
    const Atom netMoveResize = XInternAtom(display, "_NET_WM_MOVERESIZE", false);
    xev.xclient.type = ClientMessage;
    xev.xclient.message_type = netMoveResize;
    xev.xclient.display = display;
    xev.xclient.window = winId;
    xev.xclient.format = 32;

    const auto globalPos = QCursor::pos();
    xev.xclient.data.l[0] = globalPos.x();
    xev.xclient.data.l[1] = globalPos.y();
    xev.xclient.data.l[2] = _NET_WM_MOVERESIZE_MOVE;
    xev.xclient.data.l[3] = Button1;
    xev.xclient.data.l[4] = 1;
    XUngrabPointer(display, QX11Info::appTime());

    XSendEvent(display,
               QX11Info::appRootWindow(screen),
               false,
               SubstructureRedirectMask | SubstructureNotifyMask,
               &xev);
    XFlush(display);
}

void StupidWindow::setMargins(unsigned int i) {
    if (!this->horizontalLayout) {
        return;
    }
    this->horizontalLayout->setContentsMargins(i, i, i, i);

    const auto display = QX11Info::display();
    const auto winId = this->winId();

    QString s;
    QTextStream ts(&s);
    ts << i;

    const auto qByteArray = s.toLatin1();
    const Atom deepinShadow = XInternAtom(display, "DEEPIN_WINDOW_SHADOW", false);
    const auto result = XChangeProperty(
            display,
            winId,
            deepinShadow, // property
            XA_STRING, // type
            8,
            PropModeReplace,
            (const unsigned char*)qByteArray.constData(), // data
            strlen(qByteArray) // nelements
    );
    if (!result) {
        qWarning() << "XChangeProperty failed";
    }
}

QPoint StupidWindow::mapToGlobal(const QPoint& point) const {
    auto result = QWidget::mapToGlobal(point);
    const auto currentLayoutMargin = this->horizontalLayout->contentsMargins().left();
    result.setX(result.x() + currentLayoutMargin);
    result.setY(result.y() + currentLayoutMargin);
    return result;
}

void StupidWindow::changeEvent(QEvent *event) {
    QWidget::changeEvent(event);
    if (event->type() == QEvent::WindowStateChange) {
        if (this->windowState() & Qt::WindowMaximized) {
            this->setMargins(0);
        } else {
            this->setMargins(this->layoutMargin);
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

void StupidWindow::showMaximized() {
    const auto display = QX11Info::display();
    const auto winId = this->winId();
    const auto screen = QX11Info::appScreen();

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    const Atom netWmState = XInternAtom(display, "_NET_WM_STATE", false);
    const Atom verticalMaximized = XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_VERT", false);
    const Atom horizontalMaximized = XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_HORZ", false);

    xev.xclient.type = ClientMessage;
    xev.xclient.message_type = netWmState;
    xev.xclient.display = display;
    xev.xclient.window = winId;
    xev.xclient.format = 32;

    xev.xclient.data.l[0] = _NET_WM_STATE_TOGGLE;
    xev.xclient.data.l[1] = verticalMaximized;
    xev.xclient.data.l[2] = horizontalMaximized;
    xev.xclient.data.l[3] = 1;

    XSendEvent(display,
               QX11Info::appRootWindow(screen),
               false,
               SubstructureRedirectMask | SubstructureNotifyMask,
               &xev);
    XFlush(display);
}

void StupidWindow::showMinimized() {
    const auto display = QX11Info::display();
    const auto winId = this->winId();
    const auto screen = QX11Info::appScreen();

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    const Atom netWmState = XInternAtom(display, "_NET_WM_STATE", false);
    const Atom hidden = XInternAtom(display, "_NET_WM_STATE_HIDDEN", false);
    xev.xclient.type = ClientMessage;
    xev.xclient.message_type = netWmState;
    xev.xclient.display = display;
    xev.xclient.window = winId;
    xev.xclient.format = 32;

    xev.xclient.data.l[0] = _NET_WM_STATE_TOGGLE;
    xev.xclient.data.l[1] = hidden;
    xev.xclient.data.l[2] = 0;
    xev.xclient.data.l[3] = 1;

    XSendEvent(display,
        QX11Info::appRootWindow(screen),
        false,
        SubstructureRedirectMask | SubstructureNotifyMask,
        &xev
    );
    XIconifyWindow(display, winId, screen);
    XFlush(display);
}

void StupidWindow::setModal(bool on) {
    if (on) {
        this->setWindowModality(Qt::WindowModality::ApplicationModal);
    } else {
        this->setWindowModality(Qt::WindowModality::NonModal);
    }
}

void StupidWindow::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);
    this->paintOutline();
}

void StupidWindow::paintOutline() {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const auto outlinePadding = this->layout()->contentsMargins().left();
    auto rect = this->rect();
    rect.setX(outlinePadding);
    rect.setY(outlinePadding);
    rect.setWidth(rect.width() - outlinePadding);
    rect.setHeight(rect.height() - outlinePadding);

    QPainterPath path;
    path.addRoundedRect(rect, this->borderRadius, this->borderRadius);
    QPen pen;
    pen.setColor(this->borderColor);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawPath(path);
}

void StupidWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    this->polish();
}

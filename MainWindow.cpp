#include <QDebug>

#include "MainWindow.h"
#include "FilterMouseMove.h"

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

MainWindow::MainWindow(QMainWindow *parent) : QMainWindow(parent) {
    qDebug() << "Build with" << WebWidgetName;
    this->resize(960, 760);
    this->setMinimumSize(960, 760);
    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_QuitOnClose, true);
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setMouseTracking(true);

    centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralWidget");
    centralWidget->setMouseTracking(true);
    this->setCentralWidget(centralWidget);

//    centralWidget->setAutoFillBackground(true);
    horizontalLayout = new QHBoxLayout(centralWidget);
    horizontalLayout->setSpacing(1);
    horizontalLayout->setObjectName("horizontalLayout");
    horizontalLayout->setContentsMargins(resizeHandleWidth,
                                         resizeHandleWidth,
                                         resizeHandleWidth,
                                         resizeHandleWidth);

    webView = new WebView(centralWidget);
    webView->setObjectName("webView");
    webView->setUrl(QUrl("http://preview.appstore.deepin.test/"));

    // Leave event will cause problems with <horizontal-resizer>, eat leave events!
    auto filter = new FilterMouseMove(this);
    webView->installEventFilter(filter);

    horizontalLayout->addWidget(webView);

}

void MainWindow::showLessImportant() {
    // window shadow
    shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setBlurRadius(resizeHandleWidth);
    shadowEffect->setColor(Qt::red);
    shadowEffect->setOffset(0, 0);
    webView->setGraphicsEffect(shadowEffect);
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    int x = event->x();
    int y = event->y();
    CornerEdge ce = getCornerEdge(x, y);
    if (ce) {
        resizingCornerEdge = ce;
        beforeResizing = geometry();
        beforeResizingGlobalPos = event->globalPos();
        qDebug() << "mouse resize starts";
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    int x = event->x();
    int y = event->y();

    CornerEdge ce = getCornerEdge(x, y);
    if (!resizingCornerEdge) {
        updateCursor(ce);
    }
    if (resizingCornerEdge && !recalcResizingInProgress) {
        recalcResizingInProgress = true;
        QRect newRect = beforeResizing;
        int deltaX = event->globalX() - beforeResizingGlobalPos.x();
        if (resizingCornerEdge & CornerEdge::Left) {
            int newWidth = bound(minimumWidth(),
                                 beforeResizing.width() + beforeResizingGlobalPos.x() - event->globalX(),
                                 maximumWidth());
            newRect.setX(beforeResizingGlobalPos.x() + beforeResizing.width() - newWidth);
            newRect.setWidth(newWidth);
        } else if (resizingCornerEdge & CornerEdge::Right) {
            newRect.setWidth(bound(minimumWidth(), beforeResizing.width() + deltaX, maximumWidth()));
        }

        int deltaY = event->globalY() - beforeResizingGlobalPos.y();
        if (resizingCornerEdge & CornerEdge::Top) {
            int newHeight = bound(minimumHeight(),
                                  beforeResizing.height() + beforeResizing.y() - event->globalY(),
                                  maximumHeight());
            newRect.setY(beforeResizingGlobalPos.y() + beforeResizing.height() - newHeight);
            newRect.setHeight(newHeight);
        } else if (resizingCornerEdge & CornerEdge::Bottom) {
            newRect.setHeight(bound(minimumHeight(), beforeResizing.height() + deltaY, maximumHeight()));
        }

        setGeometry(newRect);
        recalcResizingInProgress = false;
    }

}

void MainWindow::mouseReleaseEvent(QMouseEvent* event) {
    if (resizingCornerEdge) {
        resizingCornerEdge = CornerEdge::Nil;
        recalcResizingInProgress = false;
        qDebug() << "mouse resize ends";
    }
}

MainWindow::~MainWindow() {

}

CornerEdge MainWindow::getCornerEdge(int x, int y) {
    QSize winSize = size();
    unsigned int ce = (unsigned int)CornerEdge::Nil;
    if (y <= resizeHandleWidth) {
        ce = ce | (unsigned int)CornerEdge::Top;
    }
    if (x <= resizeHandleWidth) {
        ce = ce | (unsigned int)CornerEdge::Left;
    }
    if (winSize.height() - resizeHandleWidth <= y) {
        ce = ce | (unsigned int)CornerEdge::Bottom;
    }
    if (winSize.width() - resizeHandleWidth <= x) {
        ce = ce | (unsigned int)CornerEdge::Right;
    }
    return (CornerEdge)ce;
}

void MainWindow::updateCursor(CornerEdge ce) {
    switch (ce) {
        case (CornerEdge::Nil): {
            unsetCursor();
            break;
        }
        case CornerEdge::Top:
        case CornerEdge::Bottom: {
            setCursor(Qt::SizeVerCursor);
            break;
        }
        case CornerEdge::Left:
        case CornerEdge::Right: {
            setCursor(Qt::SizeHorCursor);
            break;
        }
        case CornerEdge::TopLeft:
        case CornerEdge::BottomRight: {
            setCursor(Qt::SizeFDiagCursor);
            break;
        }
        case CornerEdge::TopRight:
        case CornerEdge::BottomLeft: {
            setCursor(Qt::SizeBDiagCursor);
            break;
        };
    }
}

void MainWindow::startMoving(int x, int y) {
    XEvent xev;
    Atom netMoveResize = XInternAtom(QX11Info::display(), "_NET_WM_MOVERESIZE", false);
    xev.xclient.type = ClientMessage;
    xev.xclient.message_type = netMoveResize;
    xev.xclient.display = QX11Info::display();

    xev.xclient.window = this->winId();
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = x;
    xev.xclient.data.l[1] = y;
    xev.xclient.data.l[2] = _NET_WM_MOVERESIZE_MOVE;
    xev.xclient.data.l[3] = Button1;
    xev.xclient.data.l[4] = 0;
    XUngrabPointer(QX11Info::display(), QX11Info::appTime());

    XSendEvent(QX11Info::display(),
               QX11Info::appRootWindow(QX11Info::appScreen()),
               false,
               SubstructureRedirectMask | SubstructureNotifyMask,
               &xev);

//    // release the mouse
//    QPointF releasePoint;
//    QMouseEvent* mouseUpSimulation = new QMouseEvent(QEvent::MouseButtonRelease,
//                                                     releasePoint,
//                                                     Qt::LeftButton,
//                                                     Qt::LeftButton,
//                                                     Qt::NoModifier);
//    qApp->sendEvent(webView, mouseUpSimulation);
}

void MainWindow::toggleMaximized() {
    if (this->isMaximized()) {
        this->showNormal();
    } else {
        this->showMaximized();
    }
}

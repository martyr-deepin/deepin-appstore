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

#include "common.h"

#include <QDebug>
#include <QIcon>
#include <QLayout>
#include <QEvent>
#include <QKeyEvent>

#include <DPlatformWindowHandle>
#include <DWindowManagerHelper>


#include "Shell.h"
#include "MainWindow.h"
#include "FilterMouseMove.h"


#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <QX11Info>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    qDebug() << "Build with" << WebWidgetName;
    this->setWindowIcon(QIcon::fromTheme("deepin-appstore"));
    this->resize(1028, 700);
    this->setMinimumSize(906, 680);
    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_QuitOnClose, true);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    this->setWindowFlags(Qt::FramelessWindowHint);

    this->webView = new WebView(this);

    // Leave event will cause problems with <horizontal-resizer>, eat leave events!
    const auto filter = new FilterMouseMove(this);
    this->webView->installEventFilter(filter);

    connect(this->webView, &WebView::titleChanged, [this](const QString& title) {
        if (!title.isEmpty()) {
            this->setWindowTitle(title);
//            disconnect(this->webView, &WebView::titleChanged, nullptr, nullptr);
        }
    });
    auto horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setSpacing(0);
    horizontalLayout->setMargin(0);
    horizontalLayout->setObjectName("horizontalLayout");
    this->setLayout(horizontalLayout);

    this->layout()->addWidget(this->webView);

    DPlatformWindowHandle::enableDXcbForWindow(this);

    // handle existence of composite manager
    if (Dtk::Widget::DWindowManagerHelper::instance()->hasComposite()) {
        this->setBorderRadius(5);
    } else {
        this->setBorderRadius(0);
    }
    connect(Dtk::Widget::DWindowManagerHelper::instance(), &Dtk::Widget::DWindowManagerHelper::hasCompositeChanged,
            [this]() {
                if (Dtk::Widget::DWindowManagerHelper::instance()->hasComposite()) {
                    this->setBorderRadius(5);
                } else {
                    this->setBorderRadius(0);
                }
            });
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
void MainWindow::updateCursor(CornerEdge ce) {
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

void MainWindow::setBorderRadius(int s) {
    DPlatformWindowHandle handler(this);
    handler.setWindowRadius(s);
}

void MainWindow::toggleMaximized() {
    if (this->isMaximized()) {
        this->showNormal();
    } else {
        this->showMaximized();
    }
}

void MainWindow::changeEvent(QEvent *event) {
    QWidget::changeEvent(event);
    if (event->type() == QEvent::WindowStateChange) {
        Q_EMIT this->windowStateChanged((Qt::WindowState)(int)this->windowState());
    }
}

void MainWindow::setUrl(const QUrl &url) {
    this->webView->setUrl(url);
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_F1 &&
        event->modifiers() == Qt::NoModifier) {
        const auto shell = static_cast<Shell*>(qApp);
        shell->openManual();
    };
}

MainWindow::~MainWindow() {

}

bool MainWindow::event(QEvent* event) {
    if (event->type() == QEvent::WindowDeactivate) {
        // Try hard to kill tooltips
        // https://bugzilla.deepin.io/show_bug.cgi?id=4351
        const auto shell = static_cast<Shell*>(qApp);
        shell->showTooltip("", QRect());
    }
    return QWidget::event(event);
}


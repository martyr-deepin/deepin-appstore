/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
#include "common.h"
#include <QDebug>
#include <QIcon>
#include <QLayout>
#include <QEvent>
#include <QKeyEvent>

#include "Shell.h"
#include "MainWindow.h"
#include "FilterMouseMove.h"

MainWindow::MainWindow(StupidWindow *parent) : StupidWindow(parent) {
    qDebug() << "Build with" << WebWidgetName;
    this->setWindowIcon(QIcon::fromTheme("deepin-appstore"));
    this->resize(1028, 700);
    this->setMinimumSize(906, 680);
    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_QuitOnClose, true);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

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

    this->layout()->addWidget(this->webView);
}

void MainWindow::toggleMaximized() {
    if (this->isMaximized()) {
        this->showNormal();
    } else {
        this->showMaximized();
    }
}

void MainWindow::changeEvent(QEvent *event) {
    StupidWindow::changeEvent(event);
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

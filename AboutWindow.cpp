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

#include <QDebug>
#include <QDesktopServices>

#include <DPlatformWindowHandle>
#include <QLayout>
#include <QPushButton>
#include <QKeyEvent>
#include "AboutWindow.h"
#include "WebWidget.h"

AboutWindow::AboutWindow(QWidget *parent) : QWidget(parent),
                                            contentWidth(380), contentHeight(390) {
    this->setWindowModality(Qt::WindowModality::ApplicationModal);
    this->setAutoFillBackground(true);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setFixedSize(this->contentWidth, this->contentHeight);

    this->content = new WebView(this);
    this->content->ignoreMouseMoveEvent = true;
    this->content->setFixedSize(this->contentWidth, this->contentHeight);
    this->content->setStyleSheet("QWebView { border: 0 }");

    // handle anchors
    this->content->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(this->content, &QWebView::linkClicked, [](const QUrl& url) {
        if (url.url().startsWith("http://") ||
            url.url().startsWith("https://")) {
            QDesktopServices::openUrl(url);
        }
    });

    // smaller shadow
    auto horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setSpacing(0);
    horizontalLayout->setMargin(0);
    horizontalLayout->setObjectName("horizontalLayout");
    this->setLayout(horizontalLayout);
    this->layout()->addWidget(this->content);

    const auto closeBtn = new QPushButton(this->content);
    closeBtn->setCheckable(true);
    closeBtn->setFixedSize(25, 24);
    closeBtn->move(this->contentWidth - closeBtn->width(), 0);
    closeBtn->setStyleSheet(
        "QPushButton { border: 0; background: url(':/res/close_small_normal.png'); }"
        "QPushButton:hover { background: url(':/res/close_small_hover.png'); }"
        "QPushButton:pressed { background: url(':/res/close_small_press.png'); }"
    );
    closeBtn->setFlat(true);

    connect(closeBtn, &QPushButton::clicked, [this]() {
        this->close();
    });

    DPlatformWindowHandle handler(this);
    handler.setWindowRadius(5);
}

void AboutWindow::setContent(const QString& html) {
    this->content->setHtml(html);
}

AboutWindow::~AboutWindow() {

}

void AboutWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape &&
        event->modifiers() == Qt::NoModifier) {
        this->close();
    }
}

#include <QDebug>
#include <QDesktopServices>

#include <QLayout>
#include <QPushButton>
#include <QKeyEvent>
#include "StupidWindow.h"
#include "AboutWindow.h"
#include "TextBrowser.h"

AboutWindow::AboutWindow(QWidget *parent) : StupidWindow(parent),
                                            contentWidth(380), contentHeight(390) {
    this->setModal(true);
    this->setAutoFillBackground(true);
    this->setWindowFlags(Qt::Dialog | this->windowFlags());
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setFixedSize(this->contentWidth, this->contentHeight);
    this->setStyleSheet("AboutWindow { background: transparent }");

    this->content = new TextBrowser(this);
    this->content->setTextInteractionFlags(Qt::LinksAccessibleByMouse |
                                           Qt::LinksAccessibleByKeyboard);
    this->content->setFixedSize(this->contentWidth, this->contentHeight);
    this->content->setStyleSheet("QTextBrowser { border: 0 }");

    // handle anchors
    this->content->setOpenLinks(false);
    connect(this->content, &TextBrowser::anchorClicked, [](const QUrl& url) {
        if (url.url().startsWith("http://") ||
            url.url().startsWith("https://")) {
            QDesktopServices::openUrl(url);
        }
    });

    // smaller shadow
    this->shadowOffsetY = 4;

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

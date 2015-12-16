#include <QDebug>
#include <cassert>
#include <QDesktopServices>

#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPushButton>
#include "AboutWindow.h"
#include "TextBrowser.h"

AboutWindow::AboutWindow(QWidget *parent) : QDialog(parent),
                                            layoutMargin(25),
                                            shadowRadius(12),
                                            borderRadius(3),
                                            contentWidth(355), contentHeight(340) {
    this->setModal(true);
    this->setAutoFillBackground(true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setFixedSize(this->contentWidth + 2 * this->layoutMargin,
                       this->contentHeight + 2 * this->layoutMargin);
    this->setStyleSheet("AboutWindow { background: transparent }");

    const auto horizontalLayout = new QHBoxLayout(this);
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

    horizontalLayout->setSpacing(0);
    horizontalLayout->setObjectName("horizontalLayout");
    horizontalLayout->setContentsMargins(this->layoutMargin,
                                         this->layoutMargin,
                                         this->layoutMargin,
                                         this->layoutMargin);
    horizontalLayout->addWidget(this->content);

    const auto closeBtn = new QPushButton(this);
    closeBtn->setCheckable(true);
    closeBtn->setFixedSize(25, 24);
    closeBtn->move(this->layoutMargin + this->contentWidth - closeBtn->width(),
                   this->layoutMargin);
    closeBtn->setStyleSheet(
        "QPushButton { border: 0; background: url(':/res/close_small_normal.png'); }"
        "QPushButton:hover { background: url(':/res/close_small_hover.png'); }"
        "QPushButton:pressed { background: url(':/res/close_small_press.png'); }"
    );
    closeBtn->setFlat(true);

    connect(closeBtn, &QPushButton::clicked, [this]() {
        this->close();
    });
    this->polish();

}

void AboutWindow::setContent(const QString& html) {
    this->content->setHtml(html);
}

AboutWindow::~AboutWindow() {

}

void AboutWindow::polish() {
    // window shadow
    if (!this->shadowEffect) {
        this->shadowEffect = new QGraphicsDropShadowEffect(this);
        this->shadowEffect->setBlurRadius(this->shadowRadius);
        this->shadowEffect->setColor(QColor(0, 0, 0, 255 / 5));
        this->shadowEffect->setOffset(0, 4);
        this->content->setGraphicsEffect(this->shadowEffect);
    }
}

#include <QDesktopServices>
#include <QTextBrowser>
#include "AboutWindow.h"

AboutWindow::AboutWindow(QWidget *parent) : QDialog(parent) {
//    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setModal(true);
    this->setAutoFillBackground(true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setFixedSize(400, 340);
    this->content = new QTextBrowser(this);
    this->content->setTextInteractionFlags(Qt::LinksAccessibleByMouse |
                                           Qt::LinksAccessibleByKeyboard);
    this->content->setFixedSize(400, 340);

    // handle anchors
    this->content->setOpenLinks(false);
    connect(this->content, &QTextBrowser::anchorClicked, [](const QUrl& url) {
        if (url.url().startsWith("http://") ||
            url.url().startsWith("https://")) {
            QDesktopServices::openUrl(url);
        }
    });
}

void AboutWindow::setContent(QString& html) {
    this->content->setHtml(html);
}

AboutWindow::~AboutWindow() {

}

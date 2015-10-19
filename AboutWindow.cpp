#include <QDesktopServices>
#include "AboutWindow.h"

AboutWindow::AboutWindow(QWidget *parent) : QDialog(parent) {
//    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAutoFillBackground(true);
    this->setFixedSize(400, 340);
    this->content = new QTextBrowser(this);
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

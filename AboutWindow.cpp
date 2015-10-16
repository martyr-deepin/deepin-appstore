#include "AboutWindow.h"

AboutWindow::AboutWindow(QWidget *parent) : QDialog(parent) {
//    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAutoFillBackground(true);
    this->setFixedSize(500, 500);
    this->content = new QTextBrowser(this);
    this->content->setFixedSize(500, 500);
}

void AboutWindow::setContent(QString& html) {
    this->content->setHtml(html);
}

AboutWindow::~AboutWindow() {

}

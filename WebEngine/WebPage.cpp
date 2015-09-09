
//#include <QWebChannel>
#include <QLocalSocket>
#include "WebPage.h"

WebPage::WebPage(QWidget *parent) : QWebEnginePage(parent) {
    auto socket = new QLocalSocket();

}

WebPage::~WebPage() {

}

#include <QWebChannel>

#include "WebPage.h"
#include "../Bridge.h"

WebPage::WebPage(QWidget *parent) : QWebEnginePage(parent) {
    const auto webChannel = new QWebChannel(this);
    const auto bridge = new Bridge(this);
    webChannel->registerObject("bridge", bridge);
    this->setWebChannel(webChannel);
}

WebPage::~WebPage() {

}

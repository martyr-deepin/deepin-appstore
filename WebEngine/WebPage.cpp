#include <QWebChannel>
#include <QWebEngineProfile>

#include "WebPage.h"
#include "../Bridge.h"

WebPage::WebPage(QWidget *parent) : QWebEnginePage(parent) {
    const auto webChannel = new QWebChannel(this);
    const auto bridge = new Bridge(this);
    webChannel->registerObject("bridge", bridge);
    this->setWebChannel(webChannel);

    const auto profile = this->profile();
    const auto ua = profile->httpUserAgent() + " " + qApp->applicationName() + "/" + qApp->applicationVersion();
    profile->setHttpUserAgent(ua);
}

WebPage::~WebPage() {

}

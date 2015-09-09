
#include <QDebug>
#include <QWebFrame>
#include "WebPage.h"
#include "Bridge.h"

WebPage::WebPage(QWidget* parent) : QWebPage(parent) {
    QObject::connect(this->mainFrame(), &QWebFrame::javaScriptWindowObjectCleared,
                     this, &WebPage::addBridge);
}

WebPage::~WebPage() {
    if (bridge) {
        delete bridge;
        bridge = nullptr;
    }
}

void WebPage::addBridge() {
    if (bridge) {
        delete bridge;
    }
    bridge = new Bridge(this);
    this->mainFrame()->addToJavaScriptWindowObject("bridge", bridge);
}

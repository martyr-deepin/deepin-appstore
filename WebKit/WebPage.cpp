#include "common.h"

#include <QDebug>
#include <QWebFrame>
#include "WebPage.h"
#include "Bridge.h"
#include "NetworkAccessManager.h"

WebPage::WebPage(QWidget* parent) : QWebPage(parent) {
    const auto nam = new NetworkAccessManager(this);
    this->setNetworkAccessManager(nam);
    QObject::connect(this->mainFrame(), &QWebFrame::javaScriptWindowObjectCleared,
                     this, &WebPage::addBridge);
}

WebPage::~WebPage() {
    if (this->bridge) {
        delete this->bridge;
        this->bridge = nullptr;
    }
}

void WebPage::addBridge() {
    if (this->bridge) {
        delete this->bridge;
    }
    this->bridge = new Bridge(this);
    this->mainFrame()->addToJavaScriptWindowObject("bridge", this->bridge);
}

void WebPage::javaScriptConsoleMessage(const QString& message,
                                       int UNUSED(lineNumber),
                                       const QString& sourceID) {
    qDebug() << sourceID << "==> " << message;
}

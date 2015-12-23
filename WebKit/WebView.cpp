#include <QCommandLineParser>

#include "Shell.h"
#include "WebView.h"
#include "WebPage.h"

WebView::WebView(QWidget *parent) : QWebView(parent) {
    this->customPage = new WebPage(this);
    this->setPage(this->customPage);
    this->setAcceptDrops(false);
    this->setAttribute(Qt::WA_OpaquePaintEvent, true);

    const auto shell = static_cast<Shell*>(qApp);
    const auto settings = this->settings();
    settings->enablePersistentStorage(shell->basePath + "/storage");

    settings->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, false);
    settings->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    settings->setAttribute(QWebSettings::LocalStorageEnabled, true);

    if (shell->argsParser->isSet("debug")) {
        settings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    } else {
        this->setContextMenuPolicy(Qt::NoContextMenu);
    }
}

WebView::~WebView() {

}

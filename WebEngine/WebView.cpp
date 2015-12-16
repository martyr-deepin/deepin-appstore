
#include "WebView.h"
#include "WebPage.h"

WebView::WebView(QWidget *parent) : QWebEngineView(parent) {
    const auto customPage = new WebPage(this);
    this->setPage(customPage);
}

WebView::~WebView() {

}

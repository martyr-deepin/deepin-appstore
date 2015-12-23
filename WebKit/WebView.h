#ifndef SHELL_WEBKIT_WEBVIEW_H
#define SHELL_WEBKIT_WEBVIEW_H

#include <QWebView>
class WebPage;

class WebView : public QWebView {
    Q_OBJECT

public:
    explicit WebView(QWidget* parent = nullptr);
    ~WebView();

private:
    WebPage* customPage = nullptr;
};


#endif //SHELL_WEBKIT_WEBVIEW_H

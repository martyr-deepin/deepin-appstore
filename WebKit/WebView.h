#ifndef SHELL_WEBKIT_WEBVIEW_H
#define SHELL_WEBKIT_WEBVIEW_H

#include <QWebView>
class WebPage;

class WebView : public QWebView {
    Q_OBJECT

public:
    explicit WebView(QWidget* parent = nullptr);
    ~WebView();

protected:
    virtual void paintEvent(QPaintEvent*);

private:
    WebPage* customPage = nullptr;
    unsigned borderRadius;
};


#endif //SHELL_WEBKIT_WEBVIEW_H


#ifndef SHELL_WEBENGINE_VIEW_H
#define SHELL_WEBENGINE_VIEW_H

#include <QWebEngineView>

class WebView : public QWebEngineView {
    Q_OBJECT

public:
    explicit WebView(QWidget* parent = nullptr);
    ~WebView();

};


#endif //SHELL_WEBENGINE_VIEW_H


#ifndef SHELL_WEBENGINE_WEBPAGE_H
#define SHELL_WEBENGINE_WEBPAGE_H

#include <QWebEnginePage>

class WebPage : public QWebEnginePage {
    Q_OBJECT
public:
    explicit WebPage(QWidget* parent = nullptr);
    ~WebPage();
};


#endif //SHELL_WEBENGINE_WEBPAGE_H

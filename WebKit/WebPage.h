
#ifndef SHELL_WEBKIT_WEBPAGE_H
#define SHELL_WEBKIT_WEBPAGE_H

#include <QWebPage>

class WebPage : public QWebPage {
    Q_OBJECT
public:
    explicit WebPage(QWidget* parent = nullptr);
    ~WebPage();

    void javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& sourceID) Q_DECL_OVERRIDE;
public slots:
    void addBridge();

private:
    QObject* bridge = nullptr;
};


#endif //SHELL_WEBKIT_WEBPAGE_H

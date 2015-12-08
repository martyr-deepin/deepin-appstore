
#ifndef SHELL_MAINWINDOW_H
#define SHELL_MAINWINDOW_H



#include <QWidget>
#include "StupidWindow.h"
#include "WebWidget.h"

class QHBoxLayout;



class MainWindow : public StupidWindow {
    Q_OBJECT

public:
    explicit MainWindow(StupidWindow* parent = nullptr);
    ~MainWindow();

    void toggleMaximized();
    void setUrl(const QUrl& url);


signals:
    void windowStateChanged(Qt::WindowState state);

private:
    WebView* webView = nullptr;

protected:
    void changeEvent(QEvent* event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
};

#endif //SHELL_MAINWINDOW_H

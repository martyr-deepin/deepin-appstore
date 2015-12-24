
#ifndef SHELL_ABOUTWINDOW_H
#define SHELL_ABOUTWINDOW_H

#include "StupidWindow.h"
class TextBrowser;
class QGraphicsDropShadowEffect;

class AboutWindow : public StupidWindow {
    Q_OBJECT

public:
    explicit AboutWindow(QWidget* parent = nullptr);
    ~AboutWindow();

    void setContent(const QString& html);

protected:
    void keyPressEvent(QKeyEvent * event);

private:
    TextBrowser* content = nullptr;

    unsigned int contentWidth = 0;
    unsigned int contentHeight = 0;
};

#endif //SHELL_ABOUTWINDOW_H

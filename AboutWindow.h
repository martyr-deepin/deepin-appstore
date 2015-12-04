
#ifndef SHELL_ABOUTWINDOW_H
#define SHELL_ABOUTWINDOW_H

#include <QDialog>
class TextBrowser;
class QLabel;
class QGraphicsDropShadowEffect;

class AboutWindow : public QDialog {
    Q_OBJECT

public:
    explicit AboutWindow(QWidget* parent = nullptr);
    ~AboutWindow();

    void setContent(const QString& html);
private:
    TextBrowser* content = nullptr;
    QLabel* closeButton = nullptr;
    QGraphicsDropShadowEffect* shadowEffect = nullptr;
    unsigned int layoutMargin = 0;
    unsigned int shadowRadius = 0;
    unsigned int borderRadius = 0;

    unsigned int contentWidth = 0;
    unsigned int contentHeight = 0;
    void polish();
};

#endif //SHELL_ABOUTWINDOW_H

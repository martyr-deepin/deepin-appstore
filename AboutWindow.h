
#ifndef SHELL_ABOUTWINDOW_H
#define SHELL_ABOUTWINDOW_H

#include <QDialog>
#include <QTextBrowser>

class AboutWindow : public QDialog {
    Q_OBJECT

public:
    explicit AboutWindow(QWidget* parent = nullptr);
    ~AboutWindow();

    void setContent(QString& html);
private:
    QTextBrowser* content = nullptr;
};


#endif //SHELL_ABOUTWINDOW_H

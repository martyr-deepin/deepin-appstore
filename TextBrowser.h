#ifndef SHELL_TEXTBROWSER_H
#define SHELL_TEXTBROWSER_H

#include <QTextBrowser>

class TextBrowser : public QTextBrowser {
    Q_OBJECT
public:
    TextBrowser(QWidget* parent = nullptr);
    ~TextBrowser();

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    unsigned int borderRadius = 0;
};


#endif //SHELL_TEXTBROWSER_H

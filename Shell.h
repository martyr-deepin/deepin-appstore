
#ifndef SHELL_SHELL_H
#define SHELL_SHELL_H

#include <QApplication>
#include <QPoint>
#include <libdui/darrowrectangle.h>

class Shell : public QApplication {
    Q_OBJECT
public:
    Shell(int& argc, char** argv);
    ~Shell();

    void showTooltip(QString text, QPoint globalPos);
    void setTooltipVisible(bool visible);

private:
    DUI::DArrowRectangle* tooltip = nullptr;
};


#endif //SHELL_SHELL_H

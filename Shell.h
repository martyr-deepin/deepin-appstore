
#ifndef SHELL_SHELL_H
#define SHELL_SHELL_H

#include <QApplication>

class Shell : public QApplication {
    Q_OBJECT
public:
    Shell(int& argc, char** argv);
    ~Shell();
};


#endif //SHELL_SHELL_H

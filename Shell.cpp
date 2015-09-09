
#include "Shell.h"
#include "MainWindow.h"

Shell::Shell(int &argc, char **argv) : QApplication(argc, argv) {
    MainWindow* win = new MainWindow();
    win->show();
    win->showLessImportant();
}

Shell::~Shell() {

}

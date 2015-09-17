#include "Shell.h"

int main(int argc, char *argv[]) {
    Shell::setApplicationName("Deepin Store");
    Shell::setApplicationVersion("4.0");

    Shell* shell = new Shell(argc, argv);
    return shell->exec();
}

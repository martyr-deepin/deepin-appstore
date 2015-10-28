#include "Shell.h"

int main(int argc, char *argv[]) {
    Shell::setApplicationName("Deepin Store");
    Shell::setApplicationVersion("4.0");
    Shell::setOrganizationDomain("deepin.org");
    Shell::setOrganizationName("Deepin");

    Shell* shell = new Shell(argc, argv);
    return shell->exec();
}

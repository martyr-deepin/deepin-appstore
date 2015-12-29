#include <QDebug>
#include <QProcess>

#include "main.h"
#include "Shell.h"

int main(int argc, char *argv[]) {
    Shell::setApplicationName("DeepinStore");
    Shell::setApplicationDisplayName("Deepin Store");
    Shell::setApplicationVersion(SHELL_VERSION);
    Shell::setOrganizationDomain("deepin.org");
    Shell::setOrganizationName("Deepin");

    Shell shell(argc, argv);
    int result = shell.exec();
    return result;
}

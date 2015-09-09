#include "Shell.h"

int main(int argc, char *argv[]) {
    Shell* shell = new Shell(argc, argv);
    return shell->exec();
}

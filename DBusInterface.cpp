#include <QGuiApplication>
#include <QWindow>
#include "DBusInterface.h"

DBusInterface::DBusInterface(QObject* parent) : QDBusAbstractAdaptor(parent) {
    auto result = this->connection.registerService("com.deepin.appstore");
    if (!result) {
        throw "ServiceExist";
    }

    this->connection.registerObject("/",
                                    "com.deepin.appstore",
                                    this,
                                    QDBusConnection::ExportAllSlots |
                                    QDBusConnection::ExportAllProperties |
                                    QDBusConnection::ExportAllSignals);
}

DBusInterface::~DBusInterface() {
    this->connection.unregisterService("com.deepin.appstore");
}

void DBusInterface::raise() {
    qApp->topLevelWindows()[0]->raise();
}

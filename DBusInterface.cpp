#include <QGuiApplication>
#include <QWindow>
#include "DBusInterface.h"

DBusInterface::DBusInterface(QObject* parent) : QDBusAbstractAdaptor(parent) {
    auto connection = QDBusConnection::sessionBus();

    auto result = connection.registerService(QString("org.deepin.dstoreclient"));
    if (!result) {
        throw "ServiceExist";
    }

    connection.registerObject(QString("/"),
                              QString("org.deepin.dstoreclient"),
                              this,
                              QDBusConnection::ExportAllSlots |
                              QDBusConnection::ExportAllProperties |
                              QDBusConnection::ExportAllSignals);
}

DBusInterface::~DBusInterface() {

}

void DBusInterface::raise() {
    qApp->topLevelWindows()[0]->raise();
}

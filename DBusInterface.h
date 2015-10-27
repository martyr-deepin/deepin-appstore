#ifndef SHELL_DBUSINTERFACE_H
#define SHELL_DBUSINTERFACE_H

#include <QObject>
#include <QDBusConnection>
#include <QDBusAbstractAdaptor>

class DBusInterface : public QDBusAbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.appstore")
public:
    explicit DBusInterface(QObject* parent = nullptr);
    ~DBusInterface();

public slots:
    void raise();

private:
    QDBusConnection connection = QDBusConnection::sessionBus();
};


#endif //SHELL_DBUSINTERFACE_H

#ifndef SHELL_DBUSINTERFACE_H
#define SHELL_DBUSINTERFACE_H

#include <QObject>
#include <QDBusConnection>
#include <QDBusAbstractAdaptor>

class DBusInterface : public QDBusAbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.deepin.dstoreclient")
public:
    explicit DBusInterface(QObject* parent = nullptr);
    ~DBusInterface();

public slots:
    void raise();

private:

};


#endif //SHELL_DBUSINTERFACE_H

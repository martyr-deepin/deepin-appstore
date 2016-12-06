/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
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

public Q_SLOTS:
    void raise();

private:
    QDBusConnection connection = QDBusConnection::sessionBus();
};


#endif //SHELL_DBUSINTERFACE_H

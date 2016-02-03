/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
#include <QGuiApplication>
#include <QWindow>
#include "DBusInterface.h"

DBusInterface::DBusInterface(QObject* parent) : QDBusAbstractAdaptor(parent) {
    auto result = this->connection.registerService("com.deepin.appstore");
    if (!result) {
        throw std::runtime_error("The DBus service already exists");
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

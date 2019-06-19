/*
 * Copyright (C) 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DEEPIN_APPSTORE_DBUS_DBUS_VARIANT_INSTALLED_APP_INFO_H
#define DEEPIN_APPSTORE_DBUS_DBUS_VARIANT_INSTALLED_APP_INFO_H

#include <QDBusArgument>
#include <QDebug>
#include <QList>
#include <QMap>
#include <QString>
#include <QVariant>

struct InstalledAppInfo {
public:
    InstalledAppInfo();
    ~InstalledAppInfo();

    static void registerMetaType();

    inline bool operator==(const InstalledAppInfo &other) const
    {
        return this->packageName == other.packageName;
    }

    friend QDebug operator<<(QDebug debug, const InstalledAppInfo &info);
    friend QDBusArgument &operator<<(QDBusArgument &argument,
                                     const InstalledAppInfo &info);
    friend QDataStream &operator<<(QDataStream &stream,
                                   const InstalledAppInfo &info);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument,
                                           InstalledAppInfo &info);
    friend const QDataStream &operator>>(QDataStream &stream,
                                         InstalledAppInfo &info);

    QString packageName;
    QString version;
    qint64 size;
    qint64 installationTime;
    QMap<QString, QString>localeNames;
};

typedef QList<InstalledAppInfo> InstalledAppInfoList;
Q_DECLARE_METATYPE(InstalledAppInfo);
Q_DECLARE_METATYPE(InstalledAppInfoList);

#endif  // DEEPIN_APPSTORE_DBUS_DBUS_VARIANT_INSTALLED_APP_INFO_H

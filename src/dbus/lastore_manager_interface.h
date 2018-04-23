/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp com.deepin.lastore.manager.xml -p lastore_manager_interface -i dbus/dbusvariant/app_update_info.h -c LastoreManagerInterface
 *
 * qdbusxml2cpp is Copyright (C) 2016 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef LASTORE_MANAGER_INTERFACE_H
#define LASTORE_MANAGER_INTERFACE_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>
#include "dbus/dbusvariant/app_update_info.h"

/*
 * Proxy class for interface com.deepin.lastore.Manager
 */
class LastoreManagerInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "com.deepin.lastore.Manager"; }

public:
    LastoreManagerInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~LastoreManagerInterface();

    Q_PROPERTY(bool AutoClean READ autoClean)
    inline bool autoClean() const
    { return qvariant_cast< bool >(property("AutoClean")); }

    Q_PROPERTY(QList<QDBusObjectPath> JobList READ jobList)
    inline QList<QDBusObjectPath> jobList() const
    { return qvariant_cast< QList<QDBusObjectPath> >(property("JobList")); }

    Q_PROPERTY(QStringList SystemArchitectures READ systemArchitectures)
    inline QStringList systemArchitectures() const
    { return qvariant_cast< QStringList >(property("SystemArchitectures")); }

    Q_PROPERTY(bool SystemOnChanging READ systemOnChanging)
    inline bool systemOnChanging() const
    { return qvariant_cast< bool >(property("SystemOnChanging")); }

    Q_PROPERTY(QStringList UpgradableApps READ upgradableApps)
    inline QStringList upgradableApps() const
    { return qvariant_cast< QStringList >(property("UpgradableApps")); }

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<QDBusObjectPath> CleanArchives()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("CleanArchives"), argumentList);
    }

    inline QDBusPendingReply<> CleanJob(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("CleanJob"), argumentList);
    }

    inline QDBusPendingReply<QDBusObjectPath> DistUpgrade()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("DistUpgrade"), argumentList);
    }

    inline QDBusPendingReply<QDBusObjectPath> InstallPackage(const QString &in0, const QString &in1)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0) << QVariant::fromValue(in1);
        return asyncCallWithArgumentList(QStringLiteral("InstallPackage"), argumentList);
    }

    inline QDBusPendingReply<QString> PackageDesktopPath(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("PackageDesktopPath"), argumentList);
    }

    inline QDBusPendingReply<bool> PackageExists(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("PackageExists"), argumentList);
    }

    inline QDBusPendingReply<bool> PackageInstallable(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("PackageInstallable"), argumentList);
    }

    inline QDBusPendingReply<qlonglong> PackagesDownloadSize(const QStringList &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("PackagesDownloadSize"), argumentList);
    }

    inline QDBusPendingReply<> PauseJob(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("PauseJob"), argumentList);
    }

    inline QDBusPendingReply<QDBusObjectPath> PrepareDistUpgrade()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("PrepareDistUpgrade"), argumentList);
    }

    inline QDBusPendingReply<> RecordLocaleInfo(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("RecordLocaleInfo"), argumentList);
    }

    inline QDBusPendingReply<QDBusObjectPath> RemovePackage(const QString &in0, const QString &in1)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0) << QVariant::fromValue(in1);
        return asyncCallWithArgumentList(QStringLiteral("RemovePackage"), argumentList);
    }

    inline QDBusPendingReply<> SetAutoClean(bool in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("SetAutoClean"), argumentList);
    }

    inline QDBusPendingReply<> SetCurrentX11Id(const QString &in0, const QString &in1)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0) << QVariant::fromValue(in1);
        return asyncCallWithArgumentList(QStringLiteral("SetCurrentX11Id"), argumentList);
    }

    inline QDBusPendingReply<> SetLogger(const QString &in0, const QString &in1, const QString &in2)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0) << QVariant::fromValue(in1) << QVariant::fromValue(in2);
        return asyncCallWithArgumentList(QStringLiteral("SetLogger"), argumentList);
    }

    inline QDBusPendingReply<> SetRegion(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("SetRegion"), argumentList);
    }

    inline QDBusPendingReply<> StartJob(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("StartJob"), argumentList);
    }

    inline QDBusPendingReply<QDBusObjectPath> UpdatePackage(const QString &in0, const QString &in1)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0) << QVariant::fromValue(in1);
        return asyncCallWithArgumentList(QStringLiteral("UpdatePackage"), argumentList);
    }

    inline QDBusPendingReply<QDBusObjectPath> UpdateSource()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("UpdateSource"), argumentList);
    }

Q_SIGNALS: // SIGNALS
};

namespace com {
  namespace deepin {
    namespace lastore {
      typedef ::LastoreManagerInterface Manager;
    }
  }
}
#endif
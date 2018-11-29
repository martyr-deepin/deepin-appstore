#include "apt_package_manager.h"

#include <QEventLoop>
#include <QCoreApplication>

#include "dbus/dbus_consts.h"
#include "dbus/dbus_variant/app_version.h"
#include "dbus/dbus_variant/installed_app_info.h"
#include "dbus/dbus_variant/installed_app_timestamp.h"
#include "dbus/lastore_deb_interface.h"
#include "dbus/lastore_job_interface.h"

#include "services/store_daemon_manager.h"

namespace dstore
{
namespace
{


}  // namespace

class AptPackageManagerPrivate
{
public:
    AptPackageManagerPrivate(AptPackageManager *parent) :
        deb_interface_(new LastoreDebInterface(
                           kLastoreDebDbusService,
                           kLastoreDebDbusPath,
                           QDBusConnection::sessionBus(),
                           parent)), q_ptr(parent) {}


    LastoreDebInterface *deb_interface_ = nullptr;

    AptPackageManager *q_ptr;
    Q_DECLARE_PUBLIC(AptPackageManager)
};

AptPackageManager::AptPackageManager(QObject *parent) :
    PackageManagerInterface(parent), dd_ptr(new AptPackageManagerPrivate(this))
{

}

AptPackageManager::~AptPackageManager()
{

}

PackageManagerResult AptPackageManager::Query(const QStringList &packageIDs)
{
    Q_D(AptPackageManager);
    qDebug() << packageIDs;

    const QDBusPendingReply<AppVersionList> reply =
        d->deb_interface_->QueryVersion(packageIDs);

    while (!reply.isFinished()) {
        qApp->processEvents();
    }

    if (reply.isError()) {
        qDebug() << reply.error();
        return PackageManagerResult(false,
                                    reply.error().name(),
                                    reply.error().message(),
                                    "");
    }

    const AppVersionList version_list = reply.value();
    QMap<QString, Package> result;
    for (const AppVersion &version : version_list) {
        auto package_name = version.pkg_name;
        auto packageID =  package_name.split(":").first();
        // TODO: remove name
        Package pkg;
        pkg.packageURI = "dpk://deb/" + packageID;
        pkg.packageName = package_name;
        pkg.localVersion = version.installed_version;
        pkg.remoteVersion = version.remote_version;
        pkg.upgradable = version.upgradable;
        pkg.appName = packageID;
        result.insert(packageID, pkg);
    }

    const QDBusPendingReply<InstalledAppTimestampList> installTimeReply =
        d->deb_interface_->QueryInstallationTime(packageIDs);
    if (installTimeReply.isError()) {
        qDebug() << installTimeReply.error();
        return PackageManagerResult(false,
                                    installTimeReply.error().name(),
                                    installTimeReply.error().message(),
                                    "");
    }

    while (!installTimeReply.isFinished()) {
        qApp->processEvents();
    }

    const InstalledAppTimestampList timestamp_list = installTimeReply.value();

    for (const InstalledAppTimestamp &timestamp : timestamp_list) {
        auto package_name = timestamp.pkg_name;
        auto packageID =  package_name.split(":").first();
        auto pkg = result.value(package_name);
        pkg.installedTime =  timestamp.timestamp;
        result.insert(package_name, pkg);
    }

    if (packageIDs.length() == 1) {
        auto packageName = packageIDs.value(0);
        const QDBusPendingReply<qlonglong> sizeReply =
            d->deb_interface_->QueryDownloadSize(packageName);
        while (!installTimeReply.isFinished()) {
            qApp->processEvents();
        }
        if (sizeReply.isError()) {
            qDebug() << sizeReply.error();
            return PackageManagerResult(false,
                                        sizeReply.error().name(),
                                        sizeReply.error().message(),
                                        "");
        }


        const qlonglong size = sizeReply.value();
        auto pkg = result.value(packageName);
        pkg.size = size;
        result.insert(packageName, pkg);
    }

    QVariantMap data;
    for (auto &p : result) {
        data.insert(p.packageURI, p.toVariantMap());
    }
    qDebug() << data;
    return PackageManagerResult(true,
                                "",
                                "",
                                data);
}

PackageManagerResult AptPackageManager::QueryVersion(const QStringList &packageIDs)
{
    Q_D(AptPackageManager);

    const QDBusPendingReply<AppVersionList> reply =
        d->deb_interface_->QueryVersion(packageIDs);

    while (!reply.isFinished()) {
        qApp->processEvents();
    }

    if (reply.isError()) {
        return PackageManagerResult(false,
                                    reply.error().name(),
                                    reply.error().message(),
                                    "");
    }


    const AppVersionList version_list = reply.value();
    QVariantList result;
    for (const AppVersion &version : version_list) {
        auto package_name = version.pkg_name;
        auto packageID =  package_name.split(":").first();
        // TODO: remove name
        result.append(QVariantMap {
            { "dpk", "dpk://deb/" + packageID },
            { "name", packageID },
            { "localVersion", version.installed_version },
            { "remoteVersion", version.remote_version },
            { "upgradable", version.upgradable },
        });
    }

    return PackageManagerResult(true,
                                "",
                                "",
                                result);
}

PackageManagerResult AptPackageManager::QueryInstalledTime(const QStringList &packageIDs)
{
    Q_D(AptPackageManager);
    const QDBusPendingReply<InstalledAppTimestampList> reply =
        d->deb_interface_->QueryInstallationTime(packageIDs);
    if (reply.isError()) {
        return PackageManagerResult(false,
                                    reply.error().name(),
                                    reply.error().message(),
                                    "");

    }

    const InstalledAppTimestampList timestamp_list = reply.value();
    QVariantList result;
    for (const InstalledAppTimestamp &timestamp : timestamp_list) {
        auto package_name = timestamp.pkg_name;
        auto packageID =  package_name.split(":").first();
        result.append(QVariantMap {
            { "dpk", "dpk://deb/" + packageID },
            { "app", packageID },
            { "time", timestamp.timestamp },
        });
    }

    return PackageManagerResult(true,
                                "",
                                "",
                                result);
}

PackageManagerResult AptPackageManager::ListInstalled(const QStringList &packageIDs)
{
    //TODO: remove
    QMap<QString, int> apps;
    for (auto id : packageIDs) {
        apps.insert(id, 1);
    }

    Q_D(AptPackageManager);
    const QDBusPendingReply<InstalledAppInfoList> reply =
        d->deb_interface_->ListInstalled();
    if (reply.isError()) {
        return PackageManagerResult(false,
                                    reply.error().name(),
                                    reply.error().message(),
                                    "");
    }

    const InstalledAppInfoList list = reply.value();
    QVariantList result;
    for (const InstalledAppInfo &info : list) {
        auto package_name = info.pkg_name;
        auto packageID =  package_name.split(":").first();
        // TODO: remove name
        if (apps.contains(packageID)) {
            result.append("dpk://deb/" + packageID);
        }
    }
    return PackageManagerResult(true,
                                "",
                                "",
                                result);

}

void AptPackageManager::Install(const QStringList &packageIDList)
{
    Q_UNUSED(packageIDList);
}

void AptPackageManager::Remove(const QStringList &packageIDList)
{
    Q_UNUSED(packageIDList);
//    Q_D(AptPackageManager);

//    const QDBusPendingReply<QDBusObjectPath> reply =
//        d->deb_interface_->Remove(app_local_name, app_name);

//    if (reply.isError()) {
//        return PackageManagerResult(false,
//                                    reply.error().name(),
//                                    reply.error().message(),
//                                    "");
//    }

//    if (reply.isError()) {
//        return QVariantMap {
//            { kResultOk, false },
//            { kResultErrName, reply.error().name() },
//            { kResultErrMsg, reply.error().message() },
//        };
//    } else {
//        return QVariantMap {
//            { kResultOk, true },
//            { kResultErrName, "" },
//            { kResultErrMsg, "" },
//            { kResult, reply.value().path() },
//        };
//    }
//}
//    qCritical() << Q_FUNC_INFO << "Invalid app: " << app_name;
//    return QVariantMap {
//        { kResultOk, false },
//        { kResultErrName, "Invalid app_name" },
//        { kResultErrMsg, "Invalid app_name" },
//    };
}

}

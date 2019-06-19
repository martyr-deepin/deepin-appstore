#include "apt_package_manager.h"

#include <QEventLoop>
#include <QCoreApplication>

#include "base/command.h"
#include "base/launcher.h"

#include "dbus/dbus_consts.h"
#include "dbus/dbus_variant/app_version.h"
#include "dbus/dbus_variant/installed_app_info.h"
#include "dbus/dbus_variant/installed_app_timestamp.h"
#include "dbus/lastore_deb_interface.h"
#include "dbus/lastore_job_interface.h"

#include "services/store_daemon_manager.h"

#include "apt_util_worker.h"

namespace dstore
{

namespace
{

static QStringList getIDs(const QList<Package> &packages)
{
    QStringList packageIDs;
    for (auto &package : packages) {
        packageIDs << package.dpk.getID();
    }
//    qDebug() << packageIDs;
    return packageIDs;
}

}  // namespace

class AptPackageManagerPrivate
{
public:
    AptPackageManagerPrivate(AptPackageManager *parent) :
        deb_interface_(new LastoreDebInterface(
                           kLastoreDebDbusService,
                           kLastoreDebDbusPath,
                           QDBusConnection::sessionBus(),
                           parent)), q_ptr(parent)
    {
        apt_worker_ = new AptUtilWorker();
        apt_worker_thread_ = new QThread();

        apt_worker_thread_->start();
        apt_worker_->moveToThread(apt_worker_thread_);
    }

    ~AptPackageManagerPrivate()
    {
        apt_worker_thread_->quit();
        apt_worker_thread_->wait(3);
    }


    AptUtilWorker *apt_worker_ = nullptr;
    QThread *apt_worker_thread_ = nullptr;

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

PMResult AptPackageManager::Open(const QString &packageID)
{
    Q_D(AptPackageManager);
    emit d->apt_worker_->openAppRequest(packageID);
    return PMResult::warp({});
}

PMResult AptPackageManager::Query(const QList<Package> &packages)
{
    Q_D(AptPackageManager);
    auto packageIDs = getIDs(packages);

    const QDBusPendingReply<AppVersionList> reply =
        d->deb_interface_->QueryVersion(packageIDs);

    while (!reply.isFinished()) {
        qApp->processEvents();
    }

    if (reply.isError()) {
        qWarning() << reply.error();
        return PMResult::dbusError(reply.error());
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
        return PMResult::dbusError(installTimeReply.error());
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

    QVariantMap data;
    for (auto &p : result) {
        data.insert(p.packageURI, p.toVariantMap());
    }

    return PMResult::warp(data);
}

PMResult AptPackageManager::QueryDownloadSize(const QList<Package> &packages)
{
    Q_D(AptPackageManager);

    auto packageIDs = getIDs(packages);

    QMap<QString, Package> result;
    for (auto &packageName : packageIDs) {
        const QDBusPendingReply<qlonglong> sizeReply =
            d->deb_interface_->QueryDownloadSize(packageName);

        while (!sizeReply.isFinished()) {
            qApp->processEvents();
        }

        if (sizeReply.isError()) {
            qDebug() << sizeReply.error();
            return PMResult::dbusError(sizeReply.error());
        }

        Package pkg;
        auto packageID =  packageName.split(":").first();
        pkg.packageURI = "dpk://deb/" + packageID;
        pkg.packageName = packageName;
        const qlonglong size = sizeReply.value();
        pkg.size = 0;
        pkg.downloadSize = size;

        result.insert(packageName, pkg);
    }


    QVariantMap data;
    for (auto &p : result) {
        data.insert(p.packageURI, p.toVariantMap());
    }

//    qDebug() << data;
    return PMResult::warp(data);
}

PMResult AptPackageManager::QueryVersion(const QList<Package> &packages)
{
    Q_D(AptPackageManager);
    auto packageIDs = getIDs(packages);

    const QDBusPendingReply<AppVersionList> reply =
        d->deb_interface_->QueryVersion(packageIDs);

    while (!reply.isFinished()) {
        qApp->processEvents();
    }

    if (reply.isError()) {
        qDebug() << reply.error();
        return PMResult::dbusError(reply.error());
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

    return PMResult::warp(result);
}

PMResult AptPackageManager::QueryInstalledTime(const QList<Package> &packages)
{
    Q_D(AptPackageManager);
    auto packageIDs = getIDs(packages);

    const QDBusPendingReply<InstalledAppTimestampList> reply =
        d->deb_interface_->QueryInstallationTime(packageIDs);
    if (reply.isError()) {
        qDebug() << reply.error();
        return PMResult::dbusError(reply.error());
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

    return PMResult::warp(result);
}

PMResult AptPackageManager::ListInstalled(const QList<QString> &/*packageIDs*/)
{
    Q_D(AptPackageManager);
//    auto packageIDs = getIDs(packages);

    //TODO: remove
//    QMap<QString, int> apps;
//    for (auto id : packageIDs) {
//        apps.insert(id, 1);
//    }

    const QDBusPendingReply<InstalledAppInfoList> reply =
        d->deb_interface_->ListInstalled();
    if (reply.isError()) {
        qDebug() << reply.error();
        return PMResult::dbusError(reply.error());
    }

    const InstalledAppInfoList list = reply.value();
    QVariantList result;
    for (const InstalledAppInfo &info : list) {
        Package pkg;
        pkg.packageName = info.packageName;
        auto packageID =   pkg.packageName.split(":").first();
        pkg.localVersion = info.version;
        pkg.size = info.size;
        pkg.packageURI = "dpk://deb/" + packageID;
        for (auto k : info.localeNames.keys()) {
            pkg.allLocalName[k] = info.localeNames[k];
        }
        pkg.installedTime = info.installationTime;
        // TODO: remove name
//        if (apps.contains(packageID)) {
        result.append(pkg.toVariantMap());
//            qDebug() << info.pkg_name << info.size << info.version;
//            break; // for debug
//        }
    }

    return PMResult::warp(result);
}

PMResult AptPackageManager::Install(const QList<Package> &packages)
{
    Q_D(AptPackageManager);

    QMap<QString, QVariant> result;

    for (auto &package : packages) {
        qDebug() << package.packageURI << package.dpk.getID() << package.localName;
        const QDBusPendingReply<QDBusObjectPath> reply =
            d->deb_interface_->Install(package.localName, package.dpk.getID());

        while (!reply.isFinished()) {
            qApp->processEvents();
        }

        if (reply.isError()) {
            qDebug() << reply.error();
            return PMResult::dbusError(reply.error());
        }

        result.insert(package.packageURI, reply.value().path());
    }

    qDebug() << result;
    return PMResult::warp(result);

}

PMResult AptPackageManager::Remove(const QList<Package> &packages)
{
    Q_D(AptPackageManager);

    QMap<QString, QVariant> result;

    for (auto &package : packages) {
        qDebug() << package.packageURI << package.dpk.getID() << package.localName;
        const QDBusPendingReply<QDBusObjectPath> reply =
            d->deb_interface_->Remove(package.localName, package.dpk.getID());

        while (!reply.isFinished()) {
            qApp->processEvents();
        }

        if (reply.isError()) {
            qDebug() << reply.error();
            return PMResult::dbusError(reply.error());
        }

        result.insert(package.packageURI, reply.value().path());
    }

    qDebug() << result;
    return PMResult::warp(result);
}

}

#include "package_manager.h"

#include "dpk_url.h"

#include <functional>

#include <QDebug>

namespace dstore
{

class PackageManagerPrivate
{
public:
    PackageManagerPrivate(PackageManager *parent) : q_ptr(parent) {}

    typedef PMResult PMHandler(const QString &, const QStringList &);
    typedef PMResult PMPackageHandler(const QString &, const QList<Package> &);

    PMResult mergeRun(const QStringList &list, std::function<PMHandler> callback)
    {
        QStringList dpks;
        for (auto appName : list) {
            dpks.append("dpk://deb/" + appName);
        }

        QMultiHash<QString, QString> ids;
        for (auto &packageURI : dpks) {
            DpkURI dpk(packageURI);
            ids.insert(dpk.getType(), dpk.getID());
        }
        QVariantList pkgList;
        for (auto &key : pms.keys()) {
            auto idList = ids.values(key);
            auto result = callback(key, idList);
            auto data = result.data.toList();
            pkgList.append(data);
        }
        // TODO: error handle
        return PMResult(true,
                        "",
                        "",
                        pkgList);
    }

    QMap<QString, QVariant> mapRun(QList<Package> list, std::function<PMPackageHandler> callback)
    {
        QMultiHash<QString, QString> ids;
        for (auto &package : list) {
            ids.insert(package.dpk.getType(), package.dpk.getID());
        }

        QMap<QString, QVariant> results;
        for (auto &key : pms.keys()) {
            auto packageList = ids.values(key);
            auto result = callback(key, list);
            auto packageMap = result.data.toMap();
            for (auto k : packageMap.keys()) {
                results.insert(k, packageMap.value(k));
            }
        }
        return results;
    }

    PMResult run(const AppPackageList &apps, std::function<PMPackageHandler> callback)
    {
        QList<Package> packages;
        for (const auto &v : apps) {
            for (auto p : v.packages) {
                packages.append(p);
            }
        }

        auto results = mapRun(packages, callback);

        QVariantMap appResults;
        for (auto v : apps) {
            QList<Package> packageResultList;
            for (auto package : v.packages) {
                auto packageResult = results.value(package.packageURI);
                packageResultList.append(Package::fromVariantMap(packageResult.toMap()));
            }
            v.packages = packageResultList;
            appResults.insert(v.name, v.toVariantMap());
        }

        return PMResult::warp(appResults);
    }

    QMap<QString, PackageManagerInterface *> pms;

    PackageManager *q_ptr;
    Q_DECLARE_PUBLIC(PackageManager)
};

PackageManager::PackageManager(QObject *parent) :
    QObject(parent), dd_ptr(new PackageManagerPrivate(this))
{

}

PackageManager::~PackageManager()
{

}

void PackageManager::registerDpk(const QString &type, PackageManagerInterface *ifc)
{
    Q_D(PackageManager);
    d->pms.insert(type, ifc);
}

void PackageManager::Open(const AppPackage &app)
{
    Q_D(PackageManager);
    if (app.packages.length() < 1) {
        qWarning() << "app package is empty!" << app.toVariantMap();
        return;
    }

    DpkURI dpk(app.packages.value(0).packageURI);
    auto pm = d->pms.value(dpk.getType());
    if (!pm) {
        qWarning() << "app package manager can not find!" << app.toVariantMap();
        return;
    }

    pm->Open(dpk.getID());
}

PMResult PackageManager::Query(const AppPackageList &apps)
{
    Q_D(PackageManager);
    return d->run(apps, [ & ](const QString & type, const QList<Package> &packages)-> PMResult {
        return d->pms.value(type)->Query(packages);
    });
}

PMResult PackageManager::QueryDownloadSize(const AppPackageList &apps)
{
    Q_D(PackageManager);
    return d->run(apps, [ & ](const QString & type, const QList<Package> &packages)-> PMResult {
        return d->pms.value(type)->QueryDownloadSize(packages);
    });
}

PMResult PackageManager::Install(const AppPackageList &apps)
{
    Q_D(PackageManager);
    QList<Package> packages;
    for (const auto &v : apps) {
        for (auto package : v.packages) {
            packages.append(package);
            qDebug() << package.packageURI << package.dpk.getID() << package.localName;
        }
    }

    auto results = d->mapRun(packages,
    [ & ](const QString & type, const QList<Package> &packages)-> PMResult {
        return d->pms.value(type)->Install(packages);
    });

    QStringList paths;
    for (auto v : results) {
        paths << v.toString();
    }

    return PMResult::warp(paths);
}

PMResult PackageManager::Remove(const AppPackageList &apps)
{

    Q_D(PackageManager);
    QList<Package> packages;
    for (const auto &v : apps) {
        for (auto package : v.packages) {
            packages.append(package);
            qDebug() << package.packageURI << package.dpk.getID() << package.localName;
        }
    }

    auto results = d->mapRun(packages,
    [ & ](const QString & type, const QList<Package> &packages)-> PMResult {
        return d->pms.value(type)->Remove(packages);
    });

    QStringList paths;
    for (auto v : results) {
        paths << v.toString();
    }

    return PMResult::warp(paths);
}


PMResult PackageManager::QueryVersion(const QStringList &dpks)
{
    Q_D(PackageManager);

    auto queryHandler = [ & ](const QString & key, const QStringList & idList) -> PMResult {
//        return d->pms.value(key)->QueryVersion(idList);
        Q_UNUSED(key);
        Q_UNUSED(idList);

        return PMResult::warp({});
    };

    return  d->mergeRun(dpks, queryHandler);
}

PMResult PackageManager::QueryInstalledTime(const QStringList &dpks)
{
    Q_D(PackageManager);
    auto queryHandler = [ & ](const QString & key, const QStringList & idList) -> PMResult {
//        return d->pms.value(key)->QueryInstalledTime(idList);  Q_UNUSED(key);
        Q_UNUSED(key);
        Q_UNUSED(idList);
        return PMResult::warp({});
    };

    return  d->mergeRun(dpks, queryHandler);
}

PMResult PackageManager::ListInstalled(const QStringList &packageID)
{
    Q_D(PackageManager);
    auto queryHandler = [ & ](const QString & key, const QStringList & idList) -> PMResult {
        return d->pms.value(key)->ListInstalled(idList);
    };

    return  d->mergeRun(packageID, queryHandler);
}

}

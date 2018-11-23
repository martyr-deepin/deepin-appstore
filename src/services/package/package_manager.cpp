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

    typedef PackageManagerResult PMHandler(const QString &, const QStringList &);

    PackageManagerResult mergeRun(const QStringList &list, std::function<PMHandler> callback)
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
        return PackageManagerResult(true,
                                    "",
                                    "",
                                    pkgList);
    }

    PackageManagerResult mapRun(const QStringList &list, std::function<PMHandler> callback)
    {
        QMultiHash<QString, QString> ids;
        for (auto &packageURI : list) {
            DpkURI dpk(packageURI);
//            qDebug() << packageURI << dpk.getType() << dpk.getID();
            ids.insert(dpk.getType(), dpk.getID());
        }
//        qDebug() << pms.keys();
        QVariantMap pkgList;
        for (auto &key : pms.keys()) {
            auto idList = ids.values(key);
            auto result = callback(key, idList);
            auto data = result.data.toMap();
            for (auto k : data.keys()) {
//                pkgList.insert(QString("dpk://%1/%2").arg(key).arg(k), data.value(k));
                pkgList.insert(k, data.value(k));
            }
        }
        // TODO: error handle
        return PackageManagerResult(true,
                                    "",
                                    "",
                                    pkgList);
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

PackageManagerResult PackageManager::Query(const AppPackageList &apps)
{
    Q_D(PackageManager);

    // unpack apps
    QMap<QString, QString> package2app;
    for (const auto &v : apps) {
        for (auto p : v.packages) {
            package2app.insert(p.packageURI, v.name);
        }
    }
    auto queryHandler = [ & ](const QString & key, const QStringList & idList) -> PackageManagerResult {
        auto data = d->pms.value(key)->Query(idList);
        qDebug() << data.errMsg << data.data;
        return data;
    };

    auto result = d->mapRun(package2app.keys(), queryHandler);

    auto packages = result.data.toMap();
    QMap<QString, AppPackage> retApps;
    for (auto v : apps) {
        v.packages.clear();
        retApps.insert(v.name, v);
    }

    for (auto k : packages.keys()) {
        auto pkg = Package::fromVariantMap(packages.value(k).toMap());
        auto appName = package2app.value(pkg.packageURI);
        auto app = retApps.value(appName);
        app.packages.append(pkg);
        retApps.insert(app.name, app);
    }

    QVariantMap data;
    for (auto k : retApps.keys()) {
        data.insert(k, retApps.value(k).toVariantMap());
    }

    result.data = data;
    return result;
}

PackageManagerResult PackageManager::QueryVersion(const QStringList &dpks)
{
    Q_D(PackageManager);

    auto queryHandler = [ & ](const QString & key, const QStringList & idList) -> PackageManagerResult {
        return d->pms.value(key)->QueryVersion(idList);
    };

    return  d->mergeRun(dpks, queryHandler);
}

PackageManagerResult PackageManager::QueryInstalledTime(const QStringList &dpks)
{
    Q_D(PackageManager);
    auto queryHandler = [ & ](const QString & key, const QStringList & idList) -> PackageManagerResult {
        return d->pms.value(key)->QueryInstalledTime(idList);
    };

    return  d->mergeRun(dpks, queryHandler);
}

PackageManagerResult PackageManager::ListInstalled(const QStringList &packageID)
{

    Q_D(PackageManager);
    auto queryHandler = [ & ](const QString & key, const QStringList & idList) -> PackageManagerResult {
        return d->pms.value(key)->ListInstalled(idList);
    };

    return  d->mergeRun(packageID, queryHandler);
}

void PackageManager::Install(const QStringList &dpks)
{
    Q_D(PackageManager);
    auto queryHandler = [ & ](const QString & key, const QStringList & idList) -> PackageManagerResult {
        d->pms.value(key)->Install(idList);
        return PackageManagerResult(true, "", "", "");
    };
    d->mergeRun(dpks, queryHandler);
}

void PackageManager::Remove(const QStringList &dpks)
{
    Q_D(PackageManager);
    auto queryHandler = [ & ](const QString & key, const QStringList & idList) -> PackageManagerResult {
        d->pms.value(key)->Remove(idList);
        return PackageManagerResult(true, "", "", "");
    };
    d->mergeRun(dpks, queryHandler);
}

}

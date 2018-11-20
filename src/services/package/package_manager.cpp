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
            pkgList.append(result.data.toList());
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
    auto queryHandler = [ & ](const QString & key, const QStringList &idList) -> PackageManagerResult {
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

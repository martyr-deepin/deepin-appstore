#pragma once

#include <QObject>
#include <QScopedPointer>

#include "package_manager_interface.h"

namespace dstore
{
class PackageManagerInterface;
class PackageManagerPrivate;
class PackageManager : public QObject
{
    Q_OBJECT
public:
    explicit PackageManager(QObject *parent = Q_NULLPTR);
    ~PackageManager();

    void registerDpk(const QString &type, PackageManagerInterface *);
Q_SIGNALS:

public Q_SLOTS:
    void Open(const AppPackage &app);

    PMResult Query(const AppPackageList &apps);

    PMResult QueryDownloadSize(const AppPackageList &apps);

    PMResult ListInstalled(const QStringList &packageID);

    PMResult Install(const AppPackageList &apps);

    PMResult Remove(const AppPackageList &apps);

    //TODO remove
    PMResult QueryVersion(const QStringList &packageID);
    PMResult QueryInstalledTime(const QStringList &packageID);

private:
    QScopedPointer<PackageManagerPrivate> dd_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(dd_ptr), PackageManager)
};
}

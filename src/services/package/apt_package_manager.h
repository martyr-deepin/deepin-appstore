#pragma once

#include <QObject>
#include <QScopedPointer>

#include "package_manager_interface.h"

namespace dstore
{

class AptPackageManagerPrivate;
class AptPackageManager : public PackageManagerInterface
{
    Q_OBJECT
public:
    explicit AptPackageManager(QObject *parent = Q_NULLPTR);
    ~AptPackageManager() override;

Q_SIGNALS:

public Q_SLOTS:
    virtual PMResult Open(const QString &packageID) override;
    virtual PMResult Query(const QList<Package> &packages) override;
    virtual PMResult QueryDownloadSize(const QList<Package> &packages) override;
    virtual PMResult QueryVersion(const QList<Package> &packages) override;
    virtual PMResult QueryInstalledTime(const QList<Package> &packages) override;

    virtual PMResult ListInstalled(const QList<QString> &packageIDs) override;

    virtual PMResult Install(const QList<Package> &packages) override;
    virtual PMResult Remove(const QList<Package> &packages) override;

private:
    QScopedPointer<AptPackageManagerPrivate> dd_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(dd_ptr), AptPackageManager)
};

}

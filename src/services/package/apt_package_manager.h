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
    virtual PackageManagerResult QueryVersion(const QStringList &packageIDs) override;
    virtual PackageManagerResult QueryInstalledTime(const QStringList &packageIDs) override;
    virtual PackageManagerResult ListInstalled() override;

    virtual void Install(const QStringList &packageIDList) override;
    virtual void Remove(const QStringList &packageIDList) override;

private:
    QScopedPointer<AptPackageManagerPrivate> dd_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(dd_ptr), AptPackageManager)
};

}

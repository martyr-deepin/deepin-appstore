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
    PackageManagerResult QueryVersion(const QStringList &packageID);

    PackageManagerResult QueryInstalledTime(const QStringList &packageID);

    PackageManagerResult ListInstalled();

    void Install(const QStringList &packageIDList);

    void Remove(const QStringList &packageIDList);

private:
    QScopedPointer<PackageManagerPrivate> dd_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(dd_ptr), PackageManager)
};
}

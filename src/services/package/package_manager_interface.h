#pragma once

#include <QMap>
#include <QVariant>
#include <QObject>
#include <QScopedPointer>
#include <QDBusError>

#include "dpk_url.h"

namespace dstore
{

class Package
{
public:
    ~Package()
    {
        installedTime = 0;
        size = 0;
        downloadSize = 0;
        upgradable = false;
    }

    QString packageURI;
    QString packageName;
    QString appName;
    QString localVersion;
    QString remoteVersion;
    qlonglong installedTime;
    qlonglong size;
    qlonglong downloadSize;
    bool upgradable;

    static Package fromVariantMap(const QVariantMap &json);
    QVariantMap toVariantMap() const;

//private:
    QString localName;
    DpkURI dpk;
};

typedef QMap<QString, Package> PackageMap;

struct AppPackage {
    QString         name;
    QString         localName;
//    QStringList     packageURI;
    QList<Package>  packages;

    static AppPackage fromVariantMap(const QVariantMap &json);
    QVariantMap toVariantMap() const;
};

typedef QList<AppPackage> AppPackageList;

struct PMResult {
    PMResult(bool success,
             QString errName,
             QString errMsg,
             QVariant data):
        success(success), errName(errName), errMsg(errMsg), data(data)
    {
    }

    static PMResult warp(const QVariant &data);
    static PMResult dbusError(const QDBusError &err);

    bool success;
    QString errName;
    QString errMsg;
    QVariant data;
};

typedef QMap<QString, PMResult> PMResultMap;

class PackageManagerInterface : public QObject
{
    Q_OBJECT
public:
    explicit PackageManagerInterface(QObject *parent = Q_NULLPTR);

Q_SIGNALS:

public Q_SLOTS:

    virtual PMResult Open(const QString &packageID) = 0;

    /*!
     * \brief Query
     */
    virtual PMResult Query(const QList<Package> &packageIDs) = 0;

    /*!
     * \brief QueryDownloadSize
     */
    virtual PMResult QueryDownloadSize(const QList<Package> &packageIDs) = 0;

    virtual PMResult QueryVersion(const QList<Package> &packageIDs) = 0;

    /*!
     * \brief QueryRemote
     */
    virtual PMResult QueryInstalledTime(const QList<Package> &packageIDs) = 0;

    /*!
     * \brief ListInstalled
     * \return
     */
    virtual PMResult ListInstalled(const QList<QString> &packageIDs) = 0;

    /*!
     * \brief Async install package, return immediately.
     * \param packageIDList
     */
    virtual PMResult Install(const QList<Package> &packageIDList) = 0;

    /*!
     * \brief Remove
     * \param packageIDList
     */
    virtual PMResult Remove(const QList<Package> &packageIDList) = 0;
};

}

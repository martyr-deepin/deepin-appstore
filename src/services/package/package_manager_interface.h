#pragma once

#include <QMap>
#include <QVariant>
#include <QObject>
#include <QScopedPointer>

namespace dstore
{

struct Package {
    QString packageURI;
    QString packageName;
    QString appName;
    QString localVersion;
    QString remoteVersion;
    qlonglong installedTime;
    qlonglong size;
    bool upgradable;

    static Package fromVariantMap(const QVariantMap &json);
//    static Package fromJson(const QByteArray &json);
    QVariantMap toVariantMap() const;
};

struct AppPackage {
    QString         name;
    QString         localName;
//    QStringList     packageURI;
    QList<Package>  packages;

    static AppPackage fromVariantMap(const QVariantMap &json);
    QVariantMap toVariantMap() const;
};

typedef QList<AppPackage> AppPackageList;

struct PackageManagerResult {
    PackageManagerResult(bool success,
                         QString errName,
                         QString errMsg,
                         QVariant data):
        success(success), errName(errName), errMsg(errMsg), data(data)
    {
    }

    bool success;
    QString errName;
    QString errMsg;
    QVariant data;
};


class PackageManagerInterface : public QObject
{
    Q_OBJECT
public:
    explicit PackageManagerInterface(QObject *parent = Q_NULLPTR);

Q_SIGNALS:

public Q_SLOTS:
    /*!
     * \brief Query
     */
    virtual PackageManagerResult Query(const QStringList &packageIDs) = 0;
    virtual PackageManagerResult QueryVersion(const QStringList &packageIDs) = 0;

    /*!
     * \brief QueryRemote
     */
    virtual PackageManagerResult QueryInstalledTime(const QStringList &packageIDs) = 0;

    /*!
     * \brief ListInstalled
     * \return
     */
    virtual PackageManagerResult ListInstalled(const QStringList &packageIDs) = 0;

    /*!
     * \brief Async install package, return immediately.
     * \param packageIDList
     */
    virtual void Install(const QStringList &packageIDList) = 0;

    /*!
     * \brief Remove
     * \param packageIDList
     */
    virtual void Remove(const QStringList &packageIDList) = 0;
};

}

#include "package_manager_interface.h"

namespace dstore
{

PackageManagerInterface::PackageManagerInterface(QObject *parent) :
    QObject(parent)
{

}

AppPackage AppPackage::fromVariantMap(const QVariantMap &json)
{
    AppPackage app;
    app.name = json.value("name").toString();
    app.localName = json.value("localName").toString();
    auto packages = json.value("packages").toList();
    for (const auto &v : packages) {
        Package pkg;
        pkg.localName = app.localName;
        pkg.packageURI = v.toMap().value("packageURI").toString();
        pkg.dpk = DpkURI(pkg.packageURI);
        app.packages.append(pkg);
    }

    return app;
}

QVariantMap AppPackage::toVariantMap() const
{
    QVariantMap obj;
    obj.insert("name", name);
    obj.insert("localName", localName);
    QVariantList packagesList;
    for (auto p : packages) {
        packagesList.append(p.toVariantMap());
    }
    obj.insert("packages", packagesList);
    return obj;
}

Package Package::fromVariantMap(const QVariantMap &obj)
{
    Package pkg;
    pkg.packageURI = obj.value("packageURI").toString();
    pkg.packageName = obj.value("packageName").toString();
    pkg.appName = obj.value("appName").toString();
    pkg.localVersion = obj.value("localVersion").toString();
    pkg.remoteVersion = obj.value("remoteVersion").toString();
    pkg.installedTime = static_cast<qlonglong>(obj.value("installedTime").toInt());
    pkg.size = static_cast<qlonglong>(obj.value("size").toInt());
    pkg.downloadSize = static_cast<qlonglong>(obj.value("downloadSize").toInt());
    pkg.upgradable = obj.value("upgradable").toBool();
    pkg.allLocalName = obj.value("allLocalName").toMap();
    return pkg;
}

QVariantMap Package::toVariantMap() const
{
    QVariantMap obj;
    obj.insert("packageURI", packageURI);
    obj.insert("packageName", packageName);
    obj.insert("appName", appName);
    obj.insert("localVersion", localVersion);
    obj.insert("remoteVersion", remoteVersion);
    obj.insert("installedTime", installedTime);
    obj.insert("upgradable", upgradable);
    obj.insert("size", size);
    obj.insert("downloadSize", downloadSize);
    obj.insert("localName", localName);
    obj.insert("allLocalName", allLocalName);
    return obj;
}

PMResult PMResult::warp(const QVariant &data)
{
    return PMResult(true, "", "", data);
}

PMResult PMResult::dbusError(const QDBusError &err)
{
    return PMResult(false, err.name(), err.message(), "");
}

}

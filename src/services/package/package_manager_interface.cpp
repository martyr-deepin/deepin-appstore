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
        pkg.packageURI = v.toMap().value("packageURI").toString();
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
    pkg.upgradable = obj.value("upgradable").toBool();
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
    return obj;
}

}

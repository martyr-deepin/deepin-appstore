/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "services/settings_manager.h"

#include <QDebug>
#include <QSettings>
#include <QDBusReply>
#include <QDBusInterface>

#include <qcef_global_settings.h>

#include "dbus/dbus_consts.h"
#include "base/file_util.h"

namespace dstore
{

namespace
{
const char kAutoInstall[] = "AutoInstall";
const char kThemeName[] = "ThemeName";
const char kWindowState[] = "WindowState";
const char kAllowShowPackageName[] = "AllowShowPackageName";

const char kSupportAot[] = "SupportAot";
const char kSupportSignin[] = "SupportSignIn";
const char kMetadataServer[] = "MetadataServer";
const char kOperationServerMap[] = "OperationServerMap";
const char kDefaultRegion[] = "DefaultRegion";
const char kAllowSwitchRegion[] = "AllowSwitchRegion";
const char kUpyunBannerVisible[] = "UpyunBannerVisible";
}

SettingsManager::SettingsManager(QObject *parent)
{
    settings_ifc_ = new QDBusInterface(
        kAppstoreDaemonService,
        kAppstoreDaemonSettingsPath,
        kAppstoreDaemonSettingsInterface,
        QDBusConnection::sessionBus(),
        parent);
    qDebug() << "connect" << kAppstoreDaemonInterface << settings_ifc_->isValid();
}

SettingsManager::~SettingsManager()
{

}

void SettingsManager::setQCefSettings(QCefGlobalSettings *settings)
{
    qcef_settings_ = settings;
}

bool SettingsManager::remoteDebug()
{
    return  qcef_settings_->remoteDebug();
}

QString SettingsManager::metadataServer() const
{
    return getSettings(kMetadataServer).toString();
}

QVariantMap SettingsManager::operationServerMap() const
{
    return getMapSettings(kOperationServerMap);
}

QString SettingsManager::defaultRegion() const
{
    return getSettings(kDefaultRegion).toString();
}

bool SettingsManager::autoInstall() const
{
    return getSettings(kAutoInstall).toBool();
}

void SettingsManager::setAutoInstall(bool autoinstall)
{
    setSettings(kAutoInstall, autoinstall);
}

QString SettingsManager::themeName() const
{
    return getSettings(kThemeName).toString();
}

void SettingsManager::setThemeName(const QString &themeName) const
{
    setSettings(kThemeName, themeName);
}

QByteArray SettingsManager::windowState() const
{
    auto base64str = getSettings(kWindowState).toString();
    return QByteArray::fromBase64(base64str.toLatin1());
}

void SettingsManager::setWindowState(QByteArray data)
{
    setSettings(kWindowState, QString(data.toBase64()));
}

bool SettingsManager::supportSignIn() const
{
    return getSettings(kSupportSignin).toBool();
}

bool SettingsManager::supportAot() const
{
    return getSettings(kSupportAot).toBool();
}


bool SettingsManager::allowSwitchRegion() const
{
    return getSettings(kAllowSwitchRegion).toBool();
}

bool SettingsManager::allowShowPackageName() const
{
    return getSettings(kAllowShowPackageName).toBool();
}

bool SettingsManager::upyunBannerVisible() const
{
    return getSettings(kUpyunBannerVisible).toBool();
}

typedef QVariantMap OperationInfo;

const QDBusArgument &operator>>(const QDBusArgument &argument,
                                OperationInfo &info)
{
    QString key;
    QString value;
    argument.beginMapEntry();
    argument >> key >> value;
    argument.endMapEntry();
    info[key] = value;
    return argument;
}


// a{ss}
QVariantMap SettingsManager::getMapSettings(const QString &key) const
{
    OperationInfo operationInfo;
    QDBusMessage reply = settings_ifc_->call("GetSettings", key);
    if (!reply.errorName().isEmpty()) {
        qWarning() << reply.errorName() << reply.errorMessage();
        return operationInfo;
    }
    QList<QVariant> outArgs = reply.arguments();
    QVariant first = outArgs.at(0);
    QDBusVariant dbvFirst = first.value<QDBusVariant>();
    QVariant vFirst = dbvFirst.variant();
    QDBusArgument dbusArgs = vFirst.value<QDBusArgument>();


    dbusArgs.beginArray();
    while (!dbusArgs.atEnd()) {
        dbusArgs >> operationInfo;
    }
    dbusArgs.endArray();
    return operationInfo;

}

QVariant SettingsManager::getSettings(const QString &key) const
{
    QDBusReply<QVariant> reply = settings_ifc_->call("GetSettings", key);
    if (reply.error().isValid()) {
        qWarning() << "getSettings failed" << key << reply.error();
    }
    return reply.value();
}

void SettingsManager::setSettings(const QString &key, const QVariant &value) const
{
    QDBusReply<void> reply = settings_ifc_->call("SetSettings", key, value);
    if (reply.error().isValid()) {
        qWarning() << "setSettings failed" << key << reply.error() << value;
    }
}

}  // namespace dstore

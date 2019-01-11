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

const char kGroupGeneral[] = "General";

const char kSupportSignin[] = "SupportSignIn";
const char kMetadataServer[] = "MetadataServer";
const char kOperationServer[] = "OperationServer";
const char kRegion[] = "Region";
const char kAutoInstall[] = "AutoInstall";
const char kThemeName[] = "ThemeName";
const char kWindowState[] = "WindowState";
const char kAllowSwitchRegion[] = "AllowSwitchRegion";
const char kUpyunBannerVisible[] = "UpyunBannerVisible";

}

SettingsManager::SettingsManager(QObject *parent)
{
    dbus_interface_ = new QDBusInterface(
        kAppstoreDaemonService,
        kAppstoreDaemonPath,
        kAppstoreDaemonInterface,
        QDBusConnection::sessionBus(),
        parent);
    qDebug() << "connect" << kAppstoreDaemonInterface << dbus_interface_->isValid();
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

QString SettingsManager::getMetadataServer() const
{
    return getSettings(kMetadataServer).toString();
}

QString SettingsManager::getOperationServer() const
{
    return getSettings(kOperationServer).toString();
}

OperationServerRegion SettingsManager::getRegion() const
{
    return static_cast<OperationServerRegion>(getSettings(kRegion).toInt());
}

void SettingsManager::setRegion(OperationServerRegion region)
{
    setSettings(kRegion, region);
}

bool SettingsManager::getAutoInstall() const
{
    return getSettings(kAutoInstall).toBool();
}

void SettingsManager::setAutoInstall(bool autoinstall)
{
    setSettings(kAutoInstall, autoinstall);
}

QString SettingsManager::getThemeName() const
{
    return getSettings(kThemeName).toString();
}

void SettingsManager::setThemeName(const QString &themeName) const
{
    setSettings(kThemeName, themeName);
}

QByteArray SettingsManager::getWindowState() const
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

bool SettingsManager::allowSwitchRegion() const
{
    return getSettings(kAllowSwitchRegion).toBool();
}

bool SettingsManager::getUpyunBannerVisible() const
{
    return getSettings(kUpyunBannerVisible).toBool();
}

QVariant SettingsManager::getSettings(const QString &key) const
{
    QDBusReply<QVariant> reply = dbus_interface_->call("GetSettings", key);
    if (reply.error().isValid()) {
        qWarning() << "getSettings failed" << key << reply.error();
    }
    return reply.value();
}

void SettingsManager::setSettings(const QString &key, const QVariant &value) const
{
    QDBusReply<void> reply = dbus_interface_->call("SetSettings", key, value);
    if (reply.error().isValid()) {
        qWarning() << "setSettings failed" << key << reply.error() << value;
    }
}

}  // namespace dstore

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

#include <QtGui/QDesktopServices>
#include <QUrl>
#include "ui/channel/settings_proxy.h"

#include "services/settings_manager.h"

namespace dstore
{

SettingsProxy::SettingsProxy(QObject *parent) : QObject(parent)
{
    this->setObjectName("SettingsProxy");
}

const QVariantMap SettingsProxy::getSetting()
{
    return QVariantMap {
        // user settings
        { "themeName", SettingsManager::instance()->themeName() },
        { "autoInstall",  SettingsManager::instance()->autoInstall() },
        { "allowShowPackageName", SettingsManager::instance()->allowShowPackageName() },

        // system settings
        { "metadataServer", SettingsManager::instance()->metadataServer() },
        { "operationServerMap", SettingsManager::instance()->operationServerMap()},
        { "defaultRegion", SettingsManager::instance()->defaultRegion() },
        { "allowSwitchRegion", SettingsManager::instance()->allowSwitchRegion() },
        { "supportSignIn", SettingsManager::instance()->supportSignIn() },
        // Check whether UPYun banner should be shown in app-detail page.
        {"upyunBannerVisible", SettingsManager::instance()->upyunBannerVisible()},

        // debug
        {"remoteDebug", SettingsManager::instance()->remoteDebug()},
    };
}


void SettingsProxy::setAutoInstall(bool autoInstall)
{
    SettingsManager::instance()->setAutoInstall(autoInstall);
}

void SettingsProxy::openUrl(const QString &url)
{
    QDesktopServices::openUrl(QUrl(url));
}


void SettingsProxy::raiseWindow()
{
    emit this->raiseWindowRequested();
}


}  // namespace dstore

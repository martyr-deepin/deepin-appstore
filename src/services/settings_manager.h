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

#ifndef DEEPIN_APPSTORE_SERVICES_SETTINGS_MANAGER_H
#define DEEPIN_APPSTORE_SERVICES_SETTINGS_MANAGER_H

#include <QObject>
#include <DSingleton>

class QDBusInterface;
class QCefGlobalSettings;

namespace dstore
{

enum OperationServerRegion {
    RegionChina = 0,
    RegionInternational = 1,
};

class SettingsManager : public QObject, public Dtk::Core::DSingleton<SettingsManager>
{
    Q_OBJECT
    friend class Dtk::Core::DSingleton<SettingsManager>;

private:
    explicit SettingsManager(QObject *parent = nullptr);
    ~SettingsManager() override;

Q_SIGNALS:

public Q_SLOTS:
    void setQCefSettings(QCefGlobalSettings *settings);
    bool remoteDebug();

    bool autoInstall() const;
    void setAutoInstall(bool autoinstall);

    QString themeName() const;
    void setThemeName(const QString &themeName) const;

    QByteArray windowState() const;
    void setWindowState(QByteArray data);

    bool allowShowPackageName() const;

    QString metadataServer() const;

    QVariantMap operationServerMap() const;
    QString defaultRegion() const;
    bool allowSwitchRegion() const;

    bool supportSignIn() const;
    bool upyunBannerVisible() const;

private:
    // TODO: use interface from dbus to xml
    QVariant getSettings(const QString &key) const;
    void setSettings(const QString &key, const QVariant &value) const;

    QDBusInterface *settings_ifc_;
    QCefGlobalSettings *qcef_settings_;
};

}  // namespace dstore

#endif  // DEEPIN_APPSTORE_SERVICES_SETTINGS_MANAGER_H

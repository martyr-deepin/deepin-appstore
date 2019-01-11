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

#ifndef DEEPIN_APPSTORE_UI_CHANNEL_SETTINGS_PROXY_H
#define DEEPIN_APPSTORE_UI_CHANNEL_SETTINGS_PROXY_H

#include <QObject>
#include <QVariantMap>

namespace dstore
{

class SettingsManager;

/**
 * Expose backend settings to web page.
 */
class SettingsProxy : public QObject
{
    Q_OBJECT
public:
    explicit SettingsProxy(QObject *parent = nullptr);
    ~SettingsProxy() override;

Q_SIGNALS:
    void raiseWindowRequested();
    void fontChangeRequested(const QString &fontFamily, int pixelSize);

public Q_SLOTS:
    bool remoteDebug();

    const QString getMetadataServer();
    const QString getOperationServer();

    bool getAutoInstall();

    /**
     * Allow auto install software
     */
    void setAutoInstall(bool autoinstall);

    /**
     * Request to open url in external web browser.
     * @param url
     */
    void openUrl(const QString &url);

    /**
     * Returns metadata server and operation server address.
     * @return
     */
    const QVariantMap getServers();

    /**
     * Raise main window.
     */
    void raiseWindow();

    /**
     * Check whether UPYun banner should be shown in app-detail page.
     * @return bool
     */
    bool upyunBannerVisible();
};

}  // namespace dstore

#endif  // DEEPIN_APPSTORE_UI_CHANNEL_SETTINGS_PROXY_H

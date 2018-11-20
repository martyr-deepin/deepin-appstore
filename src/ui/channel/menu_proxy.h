/*
 * Copyright (C) 2018 Deepin Technology Co., Ltd.
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

#ifndef DEEPIN_APPSTORE_UI_CHANNEL_MENU_PROXY_H
#define DEEPIN_APPSTORE_UI_CHANNEL_MENU_PROXY_H

#include <QObject>
#include <QVariantMap>

namespace dstore
{

/**
 * Expose tool menu methods and signals to web page.
 */
class MenuProxy : public QObject
{
    Q_OBJECT
public:
    explicit MenuProxy(QObject *parent = nullptr);
    ~MenuProxy() override;

signals:
    /**
     * This signal is emitted when Login/Logout menu item is activated.
     * @param login true if request to login, false for logout.
     */
    void loginRequested(bool login);

    void requestComment();
    void requestReward();
    void requestApps();

    /**
     * This signal is emitted when RecommendApp menu item is activated.
     */
    void recommendAppRequested();

    /**
     * this signal is emitted when user click menu
     */
    void switchThemeRequested(QString themeName);

    /**
     * @brief notify user info change when user login/logout from web page
     * @param info
     */
    void userInfoUpdated(QJsonObject info);

public slots:
    /**
     * @brief update titlebar info
     * @param info
     */
    void setUserInfo(QVariantMap info);
};

}  // namespace dstore

#endif  // DEEPIN_APPSTORE_UI_CHANNEL_MENU_PROXY_H

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

#ifndef DEEPIN_APPSTORE_UI_CHANNEL_SEARCH_PROXY_H
#define DEEPIN_APPSTORE_UI_CHANNEL_SEARCH_PROXY_H

#include <QObject>

#include "services/search_result.h"

namespace dstore
{

class SearchProxy : public QObject
{
    Q_OBJECT
public:
    explicit SearchProxy(QObject *parent = nullptr);
    ~SearchProxy() override;

Q_SIGNALS:
    /**
    * Request to open app info page
    * @param name
    */
    Q_SCRIPTABLE void requestComplement(const QString &keyword);

    /**
     * Request to open app info page
     * @param name
     */
    Q_SCRIPTABLE void openApp(const QString &appid);

    /**
     * Request to open app search result page
     * @param keyword search keyword
     * @param names
     */
    Q_SCRIPTABLE void openAppList(const QString &keyword);

public Q_SLOTS:
    /**
     * Update app list used in search service.
     * @param apps Serialized application info
     */
    Q_SCRIPTABLE void setComplementList(const QVariantList &apps);


Q_SIGNALS:
    void searchAppResult(const SearchMetaList &result);
};

}  // namespace dstore

#endif  // DEEPIN_APPSTORE_UI_CHANNEL_SEARCH_PROXY_H

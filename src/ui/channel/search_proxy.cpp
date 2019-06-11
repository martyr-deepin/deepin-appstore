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

#include "ui/channel/search_proxy.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariant>

namespace dstore
{

SearchProxy::SearchProxy(QObject *parent) : QObject(parent)
{
    this->setObjectName("SearchProxy");
}

SearchProxy::~SearchProxy()
{

}

void SearchProxy::setComplementList(const QVariantList &apps)
{
    SearchMetaList result;
    for (auto &app : apps) {
        auto var = app.toMap();
        SearchMeta meta;
        meta.name = var.value("name").toString();
        meta.local_name = var.value("description_name").toString();
        result.push_back(meta);
    }
    Q_EMIT searchAppResult(result);
}

}  // namespace dstore

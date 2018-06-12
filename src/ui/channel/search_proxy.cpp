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

namespace dstore {

SearchProxy::SearchProxy(QObject* parent) : QObject(parent) {
  this->setObjectName("SearchProxy");
}

SearchProxy::~SearchProxy() {

}

void SearchProxy::updateAppList(const QString& apps) {
  // Un-marshal app list.
  const QJsonDocument doc = QJsonDocument::fromJson(apps.toUtf8());
  if (!doc.isArray()) {
    qWarning() << Q_FUNC_INFO << "apps is not an array!";
    return;
  }
  const QJsonArray apps_array = doc.array();

  AppSearchRecordList record_list;

  for (const QJsonValue& app_value : apps_array) {
    if (!app_value.isObject()) {
      qWarning() << Q_FUNC_INFO << "app is not object:" << app_value;
      continue;
    }
    const QJsonObject app = app_value.toObject();
    const QString name = app.value("name").toString();
    const QJsonObject local_info = app.value("localInfo").toObject();
    const QJsonObject description = local_info.value("description").toObject();
    const QString local_name = description.value("name").toString();
    const QString slogan = description.value("slogan").toString();
    const QString local_desc = description.value("description").toString();
    const QJsonArray package_uri_arr = app.value("packageURI").toArray();
    QStringList package_uris;
    for (const QVariant& item : package_uri_arr.toVariantList()) {
      package_uris.append(item.toString());
    }
    record_list.append(AppSearchRecord {
      name,
      local_name,
      slogan,
      local_desc,
      package_uris,
      {},
      {},
    });
  }

  if (record_list.isEmpty()) {
    qWarning() << Q_FUNC_INFO << "record list is empty!" << apps;
  } else {
    emit this->onAppListUpdated(record_list);
  }
}

}  // namespace dstore
/*
 * Copyright (C) 2017 ~ $year Deepin Technology Co., Ltd.
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

#ifndef DEEPIN_APPSTORE_DBUS_APP_STORE_DBUS_PROXY_H
#define DEEPIN_APPSTORE_DBUS_APP_STORE_DBUS_PROXY_H

#include <QObject>

namespace dstore {

class AppStoreDbusProxy : public QObject {
  Q_OBJECT
 public:
  explicit AppStoreDbusProxy(QObject* parent = nullptr);
  ~AppStoreDbusProxy() override;

 signals:
  void openAppRequested(const QString& app_name);
  void raiseRequested();

 public slots:
  void OpenApp(const QString& app_name);
  void Raise();
};

}  // namespace dstore

#endif  // DEEPIN_APPSTORE_DBUS_APP_STORE_DBUS_PROXY_H

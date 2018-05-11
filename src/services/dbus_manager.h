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

#ifndef DEEPIN_APPSTORE_SERVICES_ARGS_PARSER_H
#define DEEPIN_APPSTORE_SERVICES_ARGS_PARSER_H

#include <QObject>

#include "dbus/dbus_variant/app_metadata.h"

namespace dstore {

class DBusManager : public QObject {
  Q_OBJECT
 public:
  explicit DBusManager(QObject* parent = nullptr);
  ~DBusManager() override;

  bool parseArguments();

  void openAppDelay();

 signals:
  void openAppRequested(const QString& app_name);
  void raiseRequested();
  void showDetailRequested(const QString& app_name);

 public slots:
  // Implement AppStore dbus service.
  void OpenApp(const QString& app_name);
  void Raise();
  void ShowDetail(const QString& app_name);

  AppMetadata GetAppMetadata(const QString& app_name);
  QString GetAppIcon(const QString& app_name);

 private:
  QString app_name_;
};

}  // namespace dstore

#endif  // DEEPIN_APPSTORE_SERVICES_ARGS_PARSER_H

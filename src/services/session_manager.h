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

#ifndef DEEPIN_APPSTORE_SERVICES_SESSION_MANAGER_H
#define DEEPIN_APPSTORE_SERVICES_SESSION_MANAGER_H

#include <QObject>

namespace dstore {

// SessionManager schedule dbus tasks, parse command line arguments and
// controls when to show main web window.
class SessionManager : public QObject {
  Q_OBJECT
 public:
  explicit SessionManager(QObject* parent = nullptr);
  ~SessionManager() override;
};

}  // namespace dstore

#endif  // DEEPIN_APPSTORE_SERVICES_SESSION_MANAGER_H

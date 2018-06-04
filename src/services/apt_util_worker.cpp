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

#include "services/apt_util_worker.h"

#include <QDebug>

#include "base/command.h"
#include "base/launcher.h"

namespace dstore {

AptUtilWorker::AptUtilWorker(QObject* parent) : QObject(parent) {
  this->setObjectName("AptUtilWorker");
  this->initConnections();
}

AptUtilWorker::~AptUtilWorker() {

}

void AptUtilWorker::initConnections() {
  connect(this, &AptUtilWorker::openAppRequest,
          this, &AptUtilWorker::openApp);
}

void AptUtilWorker::openApp(const QString& app_name) {
  // TODO(Shaohua): Convert app_name to debPackageName.
  OpenApp(app_name);
}

void OpenApp(const QString& app_name) {
  QString output;
  if (SpawnCmd("lastore-tools", {"querydesktop", app_name}, output)) {
    const QString desktop_file = output.trimmed();
    if (!ExecuteDesktopFile(desktop_file)) {
      qWarning() << Q_FUNC_INFO << "failed to launch:" << app_name;
    }
  }
}

}  // namespace dstore
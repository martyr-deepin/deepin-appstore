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

#include "launcher.h"

#include <QDebug>
#include <QProcess>
#include <QRegExp>
#include <QSettings>

namespace dstore {

const QString GetExecFromDesktop(const QString& filepath) {
  QSettings settings(filepath, QSettings::IniFormat);
  settings.beginGroup("Desktop Entry");
  if (settings.contains("Exec")) {
    QString exec = settings.value("Exec").toString();
    exec.remove(QRegExp("%."));
    exec.remove(QRegExp("^\""));
    exec.remove(QRegExp(" *$"));
    return exec;
  }
  return QString();
}

bool ExecuteDesktopFile(const QString& filepath) {
  const QString exec = GetExecFromDesktop(filepath);
  if (exec.isEmpty()) {
    qWarning() << Q_FUNC_INFO << "Failed to parse " << filepath;
    return false;
  }
  return QProcess::startDetached(exec);
}

}  // namespace
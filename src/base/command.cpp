/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#include "base/command.h"

#include <QDebug>
#include <QDir>
#include <QProcess>

namespace dstore {

bool RunScriptFile(const QStringList& args) {
  Q_ASSERT(!args.isEmpty());
  if (args.isEmpty()) {
    qCritical() << "RunScriptFile() args is empty!";
    return false;
  }

  // Change working directory.
  const QString current_dir(QFileInfo(args.at(0)).absolutePath());
  if (!QDir::setCurrent(current_dir)) {
    qCritical() << "Failed to change working directory:" << current_dir;
    return false;
  }

  // NOTE(xushaohua): If args[0] is not a script file, bash may raise
  // error message.
  return SpawnCmd("/bin/bash", args);
}

bool RunScriptFile(const QStringList& args, QString& output, QString& err) {
  Q_ASSERT(!args.isEmpty());
  if (args.isEmpty()) {
    qCritical() << "RunScriptFile() arg is empty!";
    return false;
  }

  // Change working directory.
  const QString current_dir(QFileInfo(args.at(0)).absolutePath());
  if (!QDir::setCurrent(current_dir)) {
    qCritical() << "Failed to change working directory:" << current_dir;
    return false;
  }

  // TODO(xushaohua): Remove bash
  return SpawnCmd("/bin/bash", args, output, err);
}

bool SpawnCmd(const QString& cmd, const QStringList& args) {
  QProcess process;
  process.setProgram(cmd);
  process.setArguments(args);
  // Merge stdout and stderr of subprocess with main process.
  process.setProcessChannelMode(QProcess::ForwardedChannels);
  process.start();
  // Wait for process to finish without timeout.
  process.waitForFinished(-1);
  return (process.exitStatus() == QProcess::NormalExit &&
          process.exitCode() == 0);
}

bool SpawnCmd(const QString& cmd, const QStringList& args, QString& output) {
  QString err;
  return SpawnCmd(cmd, args, output, err);
}

bool SpawnCmd(const QString& cmd, const QStringList& args,
              QString& output, QString& err) {
  QProcess process;
  process.setProgram(cmd);
  process.setArguments(args);
  process.start();
  // Wait for process to finish without timeout.
  process.waitForFinished(-1);
  output = process.readAllStandardOutput();
  err = process.readAllStandardError();
  return (process.exitStatus() == QProcess::NormalExit &&
          process.exitCode() == 0);
}

}  // namespace dstore
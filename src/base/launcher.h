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

#ifndef DEEPIN_APPSTORE_BASE_LAUNCHER_H
#define DEEPIN_APPSTORE_BASE_LAUNCHER_H

#include <QString>

namespace dstore {

/**
 * Get "Exec" field in a desktop file.
 * @param filepath absolute filepath to desktop file.
 * @return empty string if failed to parse.
 */
const QString GetExecFromDesktop(const QString& filepath);

/**
 * Execute an application defined in a desktop file.
 * @param filepath
 * @return
 */
bool ExecuteDesktopFile(const QString& filepath);

}  // namespace

#endif  // DEEPIN_APPSTORE_BASE_LAUNCHER_H

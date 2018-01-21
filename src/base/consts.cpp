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

#include "base/consts.h"

namespace dstore {

const char kAppName[] = "deepin-appstore";
const char* kAppDisplayName = "Deepin App Store";
const char kAppVersion[] = "5.0.0";
const char kIndexPage[] = DSTORE_WEB_DIR "/index.html";

QString GetCacheDir() {
  const char kAppCacheDir[] = ".cache/deepin/deepin-appstore";
  return QDir::home().absoluteFilePath(kAppCacheDir);
}

}  // namespace dman
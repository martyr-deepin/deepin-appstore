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

#include "services/settings_manager.h"

#include <QDebug>
#include <QSettings>

namespace dstore {

namespace {

QVariant GetSettingsValue(const QString& key) {
  QSettings settings(SETTINGS_FILE, QSettings::IniFormat);
  return settings.value(key);
}

}  // namespace

bool IsSignInSupported() {
  return GetSettingsValue("support_sign_in").toBool();
}

}  // namespace dstore
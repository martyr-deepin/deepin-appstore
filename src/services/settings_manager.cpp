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

#include "services/settings_manager.h"

#include <QDebug>
#include <QSettings>

#include "base/file_util.h"

namespace dstore {

namespace {

const char kSupportSigninName[] = "supportSignIn";
const char kMetaServerName[] = "metadataServer";

const char kOperationType[] = "operationType";
const char kOperationPrimaryServer[] = "operationPrimary";
const char kOperationSecondaryServer[] = "operationSecondary";
const char kOperationDefault[] = "operationDefault";
const char kRegionName[] = "currentRegion";
const char kThemeName[] = "themeName";

QVariant GetSystemSettingsValue(const QString& key) {
  QSettings settings(SETTINGS_FILE, QSettings::IniFormat);
  return settings.value(key);
}

}  // namespace

QString GetSessionSettingsFile() {
  QDir dir = QDir::home().absoluteFilePath(
      ".config/deepin/deepin-appstore");
  if (!dir.mkpath(".")) {
    qCritical() << Q_FUNC_INFO << "Failed to create settings folder";
  }
  return dir.filePath("settings.ini");
}

bool IsSignInSupported() {
  return GetSystemSettingsValue(kSupportSigninName).toBool();
}

QString GetMetadataServer() {
  return GetSystemSettingsValue(kMetaServerName).toString();
}

QString GetOperationServer() {
  if (GetRegion() == RegionInternational) {
    return GetSystemSettingsValue(kOperationSecondaryServer).toString();
  } else {
    return GetSystemSettingsValue(kOperationPrimaryServer).toString();
  }
}

void SetThemeName (const QString &themeName) {
  QSettings settings(GetSessionSettingsFile(), QSettings::IniFormat);
  settings.setValue(kThemeName, themeName);
}

QString GetThemeName() {
  QSettings settings(GetSessionSettingsFile(), QSettings::IniFormat);
  QString themeName = settings.value(kThemeName, "light").toString();
  return themeName;
}

void SetRegion(OperationServerRegion region) {
  QSettings settings(GetSessionSettingsFile(), QSettings::IniFormat);
  settings.setValue(kRegionName, static_cast<int>(region));
}

OperationServerRegion GetRegion() {
  if(GetOperationType() == OperationType::OperationCommunity) {
    const int default_region = GetSystemSettingsValue(kOperationDefault).toInt();
    QSettings settings(GetSessionSettingsFile(), QSettings::IniFormat);
    const int region = settings.value(kRegionName, default_region).toInt();
    return static_cast<OperationServerRegion>(region);
  } else {
    return OperationServerRegion::RegionChina;
  }
}

OperationType GetOperationType() {
  const int type = GetSystemSettingsValue(kOperationType).toInt();
  return static_cast<OperationType>(type);
}

bool UpyunBannerVisible() {
  switch (GetOperationType()) {
    case OperationType::OperationCommunity: {
      return GetRegion() == OperationServerRegion::RegionChina;
    }
    case OperationType::OperationProfessional: {
      return false;
    }
    case OperationType::OperationLoongson: {
      return false;
    }
    default: {
    }
  }
  return false;
}

bool AllowSwitchRegion() {
  return GetOperationType() == OperationType::OperationCommunity;
}

}  // namespace dstore

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

#include "services/dpk_link_validation.h"

#include <QList>

namespace dstore {

namespace {

const int kAppnameMaxLen = 64;

const char kPkgDeb[] = "deb";
const char kPkgFlatPak[] = "flatpak";

}  // namespace

DpkLinkValidationResult IsValidDpkLink(const QString& uri) {
  const QStringList parts = uri.split('/');
  if (parts.length() != 4) {
    return DpkLinkValidationResult::InvalidFormat;
  }

  // Case sensitive
  if (parts.at(0) != "dpk:") {
    return DpkLinkValidationResult::InvalidSchemeName;
  }

  if (!parts.at(1).isEmpty()) {
    return DpkLinkValidationResult::InvalidFormat;
  }

  if (parts.at(2) != kPkgDeb && parts.at(2) != kPkgFlatPak) {
    return DpkLinkValidationResult::InvalidPkgType;
  }

  if (parts.at(3).length() > kAppnameMaxLen) {
    return DpkLinkValidationResult::AppNameTooLong;
  }
  if (parts.at(3).isEmpty()) {
    return DpkLinkValidationResult::AppNameTooShort;
  }

  return DpkLinkValidationResult::Ok;
}

}  // namespace dstore
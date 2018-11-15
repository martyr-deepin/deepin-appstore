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

#include "dpk_url.h"

#include <QList>

namespace dstore
{

namespace
{

const int kAppNameMaxLen = 64;
const char kPkgDeb[] = "deb";
const char kPkgFlatPak[] = "flatpak";

enum class DpkURLValidationResult {
    Ok,
    InvalidFormat,
    InvalidSchemeName,
    InvalidPkgType,
    AppNameTooShort,
    AppNameTooLong,
};

}  // namespace

DpkURLValidationResult IsValidDpkLink(const QString &uri)
{
    const QStringList parts = uri.split('/');
    if (parts.length() != 4) {
        return DpkURLValidationResult::InvalidFormat;
    }

    // Case sensitive
    if (parts.at(0) != "dpk:") {
        return DpkURLValidationResult::InvalidSchemeName;
    }

    if (!parts.at(1).isEmpty()) {
        return DpkURLValidationResult::InvalidFormat;
    }

    if (parts.at(2) != kPkgDeb && parts.at(2) != kPkgFlatPak) {
        return DpkURLValidationResult::InvalidPkgType;
    }

    if (parts.at(3).length() > kAppNameMaxLen) {
        return DpkURLValidationResult::AppNameTooLong;
    }
    if (parts.at(3).isEmpty()) {
        return DpkURLValidationResult::AppNameTooShort;
    }

    return DpkURLValidationResult::Ok;
}


bool DpkURI::isValid() const
{
    return IsValidDpkLink(url) == DpkURLValidationResult::Ok;
}

QString DpkURI::getType() const
{
    return type;
}

QString DpkURI::getID() const
{
    return  id;
}


DpkURI::DpkURI(const QString &dpk)
{
    const QString schema = "dpk://";
    url = dpk;
    url.replace(dpk.indexOf(schema), schema.size(), "");
    type = url.left(url.indexOf("/"));
    id = url.right(url.length() - url.indexOf("/") - 1);

    // store dpk to url
    url = dpk;
}

}  // namespace dstore

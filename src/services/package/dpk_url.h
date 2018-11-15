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

#pragma once

#include <QString>

namespace dstore
{

/**
dpk://type/id
dpk://deb/deepin-manual
dpk://flatpak/org.deepin.flatdeb.dde-calendar
*/
class DpkURI
{
public:
    DpkURI(const QString &dpk);
    bool isValid() const;
    QString getType() const;
    QString getID() const;

private:
    QString url;
    QString type;
    QString id;
};

}  // namespace dstore

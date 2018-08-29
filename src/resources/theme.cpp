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

#include "resources/theme.h"

#include <QMap>

namespace dstore
{

unsigned int BackgroundColor(const QString &theme)
{
    QMap<QString, unsigned int> themeBackgroundColor;
    themeBackgroundColor.insert("light", 0xFFFFFFFF);
    themeBackgroundColor.insert("dark", 0xFF1C1C1C);

    return themeBackgroundColor[theme];
}

}  // namespace store

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

#include "dbus/dbus_consts.h"

namespace dstore
{

const char kAppStoreDbusPath[] = "/com/deepin/AppStore";
const char kAppStoreDbusService[] = "com.deepin.AppStore";

const char kAppstoreDaemonService[] = "com.deepin.AppStore.Daemon";
const char kAppstoreDaemonPath[] = "/com/deepin/AppStore/Metadata";
const char kAppstoreDaemonInterface[] = "com.deepin.AppStore.Metadata";

const char kAppstoreDaemonSettingsPath[] = "/com/deepin/AppStore/Settings";
const char kAppstoreDaemonSettingsInterface[] = "com.deepin.AppStore.Settings";

const char kLastoreDebDbusPath[] = "/com/deepin/AppStore/Backend";
const char kLastoreDebDbusService[] = "com.deepin.AppStore.Daemon";
const char kLastoreDebJobService[] = "com.deepin.AppStore.Daemon";

}  // namespace dstore

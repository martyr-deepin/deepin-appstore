/*
 * Copyright (C) 2017 ~ $year Deepin Technology Co., Ltd.
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

#include "ui/channel/log_proxy.h"

#include <QDebug>

namespace dstore {

LogProxy::LogProxy(QObject* parent) : QObject(parent) {

}

LogProxy::~LogProxy() {

}

void LogProxy::debug(const QString& msg) {
  qDebug() << msg;
}

void LogProxy::warn(const QString& msg) {
  qWarning() << msg;
}

void LogProxy::error(const QString& msg) {
  qCritical() << msg;
}

}  // namespace dstore
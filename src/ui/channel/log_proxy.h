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

#ifndef DEEPIN_APPSTORE_UI_CHANNEL_LOG_PROXY_H
#define DEEPIN_APPSTORE_UI_CHANNEL_LOG_PROXY_H

#include <QObject>

namespace dstore {

/**
 * This proxy object is used by web page to write log messages to local
 * log file.
 */
class LogProxy : public QObject {
  Q_OBJECT
 public:
  explicit LogProxy(QObject* parent = nullptr);
  ~LogProxy() override;

 public slots:
  void debug(const QString& msg);
  void warn(const QString& msg);
  void error(const QString& msg);
};

}  // namespace dstore

#endif  // DEEPIN_APPSTORE_UI_CHANNEL_LOG_PROXY_H

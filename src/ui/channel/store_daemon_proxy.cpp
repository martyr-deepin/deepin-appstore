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

#include "ui/channel/store_daemon_proxy.h"

#include <QDBusPendingReply>

#include "base/launcher.h"
#include "dbus/dbus_consts.h"
#include "dbus/lastore_job_interface.h"

namespace dstore
{

StoreDaemonProxy::StoreDaemonProxy(QObject *parent)
    : QObject(parent),
      manager_thread_(new QThread(this)),
      manager_(new StoreDaemonManager())
{

    this->setObjectName("StoreDaemonProxy");

    RegisterSearchMetaMetaType();

    this->initConnections();

    manager_thread_->start();
    manager_->moveToThread(manager_thread_);

}

StoreDaemonProxy::~StoreDaemonProxy()
{
    manager_thread_->quit();
    manager_thread_->wait(3);
}

void StoreDaemonProxy::initConnections()
{
    connect(manager_thread_, &QThread::finished,
            manager_, &StoreDaemonManager::deleteLater);
    connect(manager_, &StoreDaemonManager::jobListChanged,
            this, &StoreDaemonProxy::jobListChanged);
}

}  // namespace dstore

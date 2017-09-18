/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef SHELL_COMMON_H
#define SHELL_COMMON_H

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif

#include <functional>
#include <QDBusPendingCallWatcher>
#include <QDBusPendingReply>
#include <QDebug>

// T: Value type
template
<typename T> void asyncWatcherFactory(const QDBusPendingReply<T> pendingReply,
                                      std::function<void (QDBusPendingReply<T>)> onSuccess,
                                      std::function<void (QDBusError error)> onError = nullptr,
                                      std::function<void (bool success)> onDone = nullptr) {
    const auto watcher = new QDBusPendingCallWatcher(pendingReply, nullptr);
    QObject::connect(watcher, &QDBusPendingCallWatcher::finished,
                     [watcher, onSuccess, onError, onDone](QDBusPendingCallWatcher* call)  {
                         QDBusPendingReply<T> reply = *call;
                         if (reply.isError()) {
                             const auto error = reply.error();
                             if (onError) {
                                 onError(error);
                             } else {
                                 qWarning() << error.name() << error.message();
                             }
                         } else {
                             if (onSuccess) {
                                onSuccess(reply);
                             }
                         }
                         if (onDone) {
                             onDone(!reply.isError());
                         }
                         delete watcher;
                     });
}

#endif //SHELL_COMMON_H

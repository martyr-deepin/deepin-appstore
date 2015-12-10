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
                             onDone(reply.isError());
                         }
                         delete watcher;
                     });
}

#endif //SHELL_COMMON_H

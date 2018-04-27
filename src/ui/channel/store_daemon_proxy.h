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

#ifndef DEEPIN_APPSTORE_UI_STORE_DAEMON_PROXY_H
#define DEEPIN_APPSTORE_UI_STORE_DAEMON_PROXY_H

#include <QObject>
#include <QVariantMap>
class QThread;

#include "dbus/dbusvariant/app_update_info.h"
#include "dbus/dbusvariant/locale_mirror_source.h"
#include "services/store_daemon_worker.h"
class LastoreManagerInterface;
class LastoreUpdaterInterface;

namespace dstore {

class StoreDaemonProxy : public QObject {
  Q_OBJECT
 public:
  explicit StoreDaemonProxy(QObject* parent = nullptr);
  ~StoreDaemonProxy() override;

 signals:
  void isDbusConnectedReply(bool state);

  void cleanArchivesReply(const QVariantMap& result);
  void cleanJobReply(const QVariantMap& result);
  void pauseJobReply(const QVariantMap& result);
  void startJobReply(const QVariantMap& result);
  void installPackagesReply(const QVariantMap& result);
  void packageExistsReply(const QVariantMap& result);
  void packageInstallableReply(const QVariantMap& result);
  void packageDownloadSizeReply(const QVariantMap& result);
  void updatePackageReply(const QVariantMap& result);
  void removePackageReply(const QVariantMap& result);
  void jobListReply(const QVariantMap& result);
  void upgradableAppsReply(const QVariantMap& result);

  void applicationUpdateInfosReply(const QVariantMap& result);
  void getJobInfoReply(const QVariantMap& result);

 public slots:
  /**
   * Check connecting to backend app store daemon or not.
   */
  void isDBusConnected();

  // Store Manager methods:
  /**
   * apt-get clean
   * @return string, returns job path
   */
  void cleanArchives();

  /**
   * Clean up a specific job.
   * @param job
   */
  void cleanJob(const QString& job);

  /**
   * Pause a running job
   * @param job
   */
  void pauseJob(const QString& job);

  /**
   * Resume a paused job
   * @param job
   */
  void startJob(const QString& job);

  /**
   * apt-get install xxx
   * @param app_name
   */
  void installPackage(const QString& app_name);

  /**
   * Check whether this package is already installed into system.
   * @param app_name
   */
  void packageExists(const QString& app_name);

  /**
   * Check whether a specific package exists in APT store
   * @param app_name
   */
  void packageInstallable(const QString& app_name);

  /**
   * Get deb package size
   * @param app_name
   */
  void packageDownloadSize(const QString& app_name);

  /**
   * apt-get upgrade xxx
   * @param app_name
   */
  void updatePackage(const QString& app_name);

  /**
   * apt-get remove xxx
   * @param app_name
   * @return string, returns job path
   */
  void removePackage(const QString& app_name);

  /**
   * Returns all of jobs existing in backend.
   * @return stringList
   */
  void jobList();

  void upgradableApps();

  // Store Updater methods:
  void applicationUpdateInfos(const QString& language);

  /**
   * Get temporary job info.
   * * valid: bool, false if this job is invalid.
   * * id: string
   * * name: string
   * * status: string
   * * type: string
   * * speed: int64
   * * progress: double
   * * description: string
   * * cancelable: boolean
   * * packages: stringList
   */
  void getJobInfo(const QString& job);

 private:
  void initConnections();

  LastoreManagerInterface* manager_ = nullptr;
  LastoreUpdaterInterface* updater_ = nullptr;

  QThread* worker_thread_ = nullptr;
  StoreDaemonWorker* worker_ = nullptr;
};

}  // namespace dstore

#endif  // DEEPIN_APPSTORE_UI_STORE_DAEMON_PROXY_H

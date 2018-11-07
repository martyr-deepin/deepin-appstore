/*
 * Copyright (C) 2018 Deepin Technology Co., Ltd.
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

#ifndef DEEPIN_APPSTORE_SERVICES_STORE_DAEMON_MANAGER_H
#define DEEPIN_APPSTORE_SERVICES_STORE_DAEMON_MANAGER_H

#include <QObject>

#include "services/search_result.h"

class QThread;

class LastoreDebInterface;

namespace dstore {

class AptUtilWorker;

class StoreDaemonManager : public QObject {
  Q_OBJECT
 public:
  explicit StoreDaemonManager(QObject* parent = nullptr);
  ~StoreDaemonManager() override;
  
 signals:

  /**
   * Emitted when JobList property changed.
   * @param jobs
   */
  void jobListChanged(const QStringList& jobs);

  void onAppListUpdated(const AppSearchRecordList& app_list);

 public slots:
  void updateAppList(const AppSearchRecordList& app_list);

 private:
  void initConnections();

  bool hasDebPkg(const QString& app_name) const;
  bool hasFlatPak(const QString& app_name) const;

  // Maps between appName and its metadata.
  AppSearchRecordMap apps_;

  // Maps between debPkgName and appName. Many-to-Many.
  // Different appNames can refer to the same deb.
  // And one appName can contain multiple deb names.
  QMultiHash<QString, QString> deb_names_;

  // Maps between flatpakName and appName. Many-to-Many.
  QMultiHash<QString, QString> flatpak_names_;

  AptUtilWorker* apt_worker_ = nullptr;
  QThread* apt_worker_thread_ = nullptr;
  LastoreDebInterface* deb_interface_ = nullptr;

 public slots:
  void clearArchives();
  void openApp(const QString& app_name);

  /**
   * Check connecting to backend app store daemon or not.
   */
  bool isDBusConnected();

  /**
   * Clean up a specific job.
   * @param job
   */
  QVariantMap cleanJob(const QString& job);

  /**
   * Pause a running job
   * @param job
   */
  QVariantMap pauseJob(const QString& job);

  /**
   * Resume a paused job
   * @param job
   */
  QVariantMap startJob(const QString& job);

  /**
   * apt-get install xxx
   * @param app_name
   * @param app_local_name
   */
  QVariantMap installPackage(const QString& app_name, const QString& app_local_name);

  QVariantMap installedPackages();

  /**
   * Get deb package size
   * @param app_name
   */
  QVariantMap packageDownloadSize(const QString& app_name);

  /**
   * apt-get upgrade xxx
   * @param app_name
   * @param app_local_name
   */
  QVariantMap updatePackage(const QString& app_name, const QString& app_local_name);

  /**
   * apt-get remove xxx
   * @param app_name
   * @param app_local_name
   */
  QVariantMap removePackage(const QString& app_name, const QString& app_local_name);

  QVariantMap queryVersions(const QStringList& apps);

  QVariantMap queryInstalledTime(const QStringList& apps);

  /**
   * Returns all of jobs existing in backend.
   * @return stringList
   */
  QVariantMap jobList();

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
  QVariantMap getJobInfo(const QString& job);

  QVariantMap getJobsInfo(const QStringList& jobs);

  void onJobListChanged();

  QVariantMap fixError(const QString& error_type);
};

}  // namespace dstore

#endif  // DEEPIN_APPSTORE_SERVICES_STORE_DAEMON_MANAGER_H

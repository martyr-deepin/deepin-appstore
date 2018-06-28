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
  void isDbusConnectedRequest();

  void clearArchivesRequest();
  void cleanJobRequest(const QString& job);
  void pauseJobRequest(const QString& job);
  void startJobRequest(const QString& job);
  void installPackageRequest(const QString& app_name,
                             const QString& app_local_name);
  void packageDownloadSizeRequest(const QString& app_name);
  void updatePackageRequest(const QString& app_name,
                            const QString& app_local_name);
  void removePackageRequest(const QString& app_name,
                            const QString& app_local_name);
  void installedPackagesRequest();

  void queryVersionsRequest(const QString& task_id, const QStringList& apps);
  void queryInstalledTimeRequest(const QString& task_id,
                                 const QStringList& apps);
  void jobListRequest();
  void getJobInfoRequest(const QString& job);
  void getJobsInfoRequest(const QString& task_id, const QStringList& jobs);

  void openAppRequest(const QString& app_name);


  void isDbusConnectedReply(bool state);

  void cleanJobReply(const QVariantMap& result);
  void pauseJobReply(const QVariantMap& result);
  void startJobReply(const QVariantMap& result);
  void installPackageReply(const QVariantMap& result);
  void packageDownloadSizeReply(const QVariantMap& result);
  void updatePackageReply(const QVariantMap& result);
  void removePackageReply(const QVariantMap& result);

  void queryVersionsReply(const QVariantMap& result);
  void queryInstalledTimeReply(const QVariantMap& result);
  void jobListReply(const QVariantMap& result);
  void installedPackagesReply(const QVariantMap& result);
  void getJobInfoReply(const QVariantMap& result);
  void getJobsInfoReply(const QVariantMap& result);

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

 private slots:
  void clearArchives();

  /**
   * Check connecting to backend app store daemon or not.
   */
  void isDBusConnected();

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
   * @param app_local_name
   */
  void installPackage(const QString& app_name, const QString& app_local_name);

  void installedPackages();

  /**
   * Get deb package size
   * @param app_name
   */
  void packageDownloadSize(const QString& app_name);

  /**
   * apt-get upgrade xxx
   * @param app_name
   * @param app_local_name
   */
  void updatePackage(const QString& app_name, const QString& app_local_name);

  /**
   * apt-get remove xxx
   * @param app_name
   * @param app_local_name
   */
  void removePackage(const QString& app_name, const QString& app_local_name);

  void queryVersions(const QString& task_id, const QStringList& apps);

  void queryInstalledTime(const QString& task_id, const QStringList& apps);

  /**
   * Returns all of jobs existing in backend.
   * @return stringList
   */
  void jobList();

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

  void getJobsInfo(const QString& task_id, const QStringList& jobs);

  void onJobListChanged();
};

}  // namespace dstore

#endif  // DEEPIN_APPSTORE_SERVICES_STORE_DAEMON_MANAGER_H

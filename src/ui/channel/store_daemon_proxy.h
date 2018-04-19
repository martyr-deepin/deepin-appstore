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

#include "dbus/dbusvariant/app_update_info.h"
#include "dbus/dbusvariant/locale_mirror_source.h"
class LastoreManagerInterface;
class LastoreUpdaterInterface;

namespace dstore {

class StoreDaemonProxy : public QObject {
  Q_OBJECT
 public:
  explicit StoreDaemonProxy(QObject* parent = nullptr);
  ~StoreDaemonProxy() override;

 public slots:
  /**
   * Check connecting to backend app store daemon or not.
   * @return
   */
  bool isDBusConnected() const;

  // Store Manager methods:
  /**
   * apt-get clean
   * @return string, returns job path
   */
  const QVariantMap cleanArchives();

  /**
   * Clean up a specific job.
   * @param job
   * @return void
   */
  const QVariantMap cleanJob(const QString& job);

  /**
   * Pause a running job
   * @param job
   * @return void
   */
  const QVariantMap pauseJob(const QString& job);

  /**
   * Resume a paused job
   * @param job
   * @return void
   */
  const QVariantMap startJob(const QString& job);

  const QString distUpgrade();

  /**
   * apt-get install xxx
   * @param app_name
   * @return string returns job path
   */
  const QVariantMap installPackage(const QString& app_name);

  /**
   * Check whether this package is already installed into system.
   * @param app_name
   * @return boolean
   */
  const QVariantMap packageExists(const QString& app_name);

  const QString packageDesktopPath(const QString& app_name);

  /**
   * Check whether a specific package exists in APT store
   * @param app_name
   * @return boolean
   */
  const QVariantMap packageInstallable(const QString& app_name);

  /**
   * Get deb package size
   * @param app_name
   * @return longlong
   */
  const QVariantMap packageDownloadSize(const QString& app_name);

  const QString prepareDistUpgrade();
  void recordLocaleInfo(const QString& language);

  /**
   * apt-get upgrade xxx
   * @param app_name
   * @return string, returns job path
   */
  const QVariantMap updatePackage(const QString& app_name);

  const QString updateSource();

  /**
   * apt-get remove xxx
   * @param app_name
   * @return string, returns job path
   */
  const QVariantMap removePackage(const QString& app_name);

  void setAutoClean(bool enabled);
  void setRegion(const QString& region);

  // Store Manager properties:
  bool autoClean();

  /**
   * Returns all of jobs existing in backend.
   * @return stringList
   */
  const QVariantMap jobList();

  const QStringList systemArchitectures();
  bool systemOnChanging();
  const QStringList upgradableApps();

  // Store Updater methods:
  const QVariantList applicationUpdateInfos(const QString& language);
  const QVariantList listMirrorSources(const QString& language);
  void setAutoCheckUpdates(bool check);
  void setAutoDownloadUpdates(bool update);
  void setMirrorSource(const QString& id);

  // Store Manager properties:
  bool autoCheckUpdates();
  bool autoDownloadUpdates();
  const QString mirrorSource();
//  QStringList updatableApps();
//  QStringList updatablePackages();

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
  const QVariantMap getJobInfo(const QString& job);

  /**
   * Request to launch application.
   * @param app_name
   */
  void openApp(const QString& app_name);

 private:
  LastoreManagerInterface* manager_ = nullptr;
  LastoreUpdaterInterface* updater_ = nullptr;
};

}  // namespace dstore

#endif  // DEEPIN_APPSTORE_UI_STORE_DAEMON_PROXY_H

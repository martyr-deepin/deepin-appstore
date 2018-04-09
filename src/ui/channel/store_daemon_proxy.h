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

 signals:

 public slots:
  // Store Manager methods:
  QString cleanArchives();
  void cleanJob(const QString& job);
  QString distUpgrade();
  QString installPackage(const QString& job, const QString& package);
  QString packageDesktopPath(const QString& package);
  bool packageExists(const QString& package);
  bool packageInstallable(const QString& package);
  qint64 packagesDownloadSize(const QString& package);
  void pauseJob(const QString& job);
  QString prepareDistUpgrade();
  void recordLocaleInfo();
  void startJob(const QString& job);
  QString updatePackage(const QString& job, const QString& packages);
  QString updateSource();

  /**
   * Remove specific packages.
   * @param job
   * @param packages A list of package names, separated with spaces.
   * @return
   */
  QString removePackage(const QString& job, const QString& packages);
  void setAutoClean(bool enabled);
  void setRegion(const QString& region);

  // Store Manager properties:
  bool autoClean();
  QStringList jobList();
  QStringList systemArchitectures();
  bool systemOnChanging();
  QStringList upgradableApps();


  // Store Updater methods:
  AppUpdateInfoList applicationUpdateInfos(const QString& language);
  LocaleMirrorSourceList listMirrorSources(const QString& language);
  void setAutoCheckUpdates(bool check);
  void setAutoDownloadUpdates(bool update);
  void setMirrorSource(const QString& id);

  // Store Manager properties:
  bool autoCheckUpdates();
  bool autoDownloadUpdates();
  QString mirrorSource();
  QStringList updatableApps();
  QStringList updatablePackages();

 private:
  LastoreManagerInterface* store_manager_iface_ = nullptr;
  LastoreUpdaterInterface* store_updater_iface_ = nullptr;
};

}  // namespace dstore

#endif  // DEEPIN_APPSTORE_UI_STORE_DAEMON_PROXY_H

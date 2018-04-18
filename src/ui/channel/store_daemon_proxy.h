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
  /**
   * Check connecting to backend app store daemon or not.
   * @return
   */
  bool isDBusConnected() const;

  // Store Manager methods:
  QString cleanArchives();
  void cleanJob(const QString& job);
  QString distUpgrade();
  QString installPackage(const QString& package);
  QString packageDesktopPath(const QString& package);
  bool packageExists(const QString& package);
  bool packageInstallable(const QString& package);
  qlonglong packagesDownloadSize(const QStringList& packages);
  void pauseJob(const QString& job);
  QString prepareDistUpgrade();
  void recordLocaleInfo(const QString& language);
  void startJob(const QString& job);
  QString updatePackage(const QString& package);
  QString updateSource();
  QString removePackage(const QString& package);
  void setAutoClean(bool enabled);
  void setRegion(const QString& region);

  // Store Manager properties:
  bool autoClean();
  QStringList jobList();
  QStringList systemArchitectures();
  bool systemOnChanging();
  QStringList upgradableApps();


  // Store Updater methods:
  const QVariantList applicationUpdateInfos(const QString& language);
  const QVariantList listMirrorSources(const QString& language);
  void setAutoCheckUpdates(bool check);
  void setAutoDownloadUpdates(bool update);
  void setMirrorSource(const QString& id);

  // Store Manager properties:
  bool autoCheckUpdates();
  bool autoDownloadUpdates();
  QString mirrorSource();
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
   * * description: string
   * * cancelable: boolean
   * * packages: stringList
   */
   const QVariantMap getJobInfo(const QString& job);
 private:
  LastoreManagerInterface* manager_ = nullptr;
  LastoreUpdaterInterface* updater_ = nullptr;
};

}  // namespace dstore

#endif  // DEEPIN_APPSTORE_UI_STORE_DAEMON_PROXY_H

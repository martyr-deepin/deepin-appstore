/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
#ifndef SHELL_LASTOREBRIDGE_H
#define SHELL_LASTOREBRIDGE_H

#include <lastore-daemon.h>

using namespace dbus::common;
using namespace dbus::objects;
using namespace dbus::objects::com::deepin::lastore;

struct JobCombo {
    Job* object;
    QVariantMap info;
    unsigned int nCallback;
};

class LAStoreBridge : public QObject {
    Q_OBJECT

public:
    explicit LAStoreBridge(QObject* parent = nullptr);
    ~LAStoreBridge();

public:
    Q_SLOT void installApp(const QString& appId);
    Q_SLOT void launchApp(const QString& pkgId);
    Q_SLOT void updateApp(const QString& appId);
    Q_SLOT void startJob(const QString& jobId);
    Q_SLOT void pauseJob(const QString& jobId);
    Q_SLOT void cancelJob(const QString& jobId);

    Q_SLOT void askSystemArchitectures();
    Q_SLOT void askRunningJobs();
    Q_SLOT void askInstallingApps();
    Q_SLOT void askUpdatableApps();
    Q_SLOT void askJobPaths();
    Q_SLOT void askOverallProgress();

    Q_SLOT void askJobInfo(const QString& jobPath);

    Q_SLOT void askAppInstalled(const QString& pkgId);
    Q_SLOT void askDownloadSize(const QString& pkgId);

Q_SIGNALS:
    void jobPathsAnswered(QStringList jobPaths);
    void runningJobsAnswered(QStringList runningJobs);
    void updatableAppsAnswered(QStringList updatableApps);
    void installingAppsAnswered(QStringList installingApps);
    void systemArchitecturesAnswered(QStringList archs);
    void appInstalledAnswered(QString pkgId, bool installed);
    void downloadSizeAnswered(QString pkgId, long long size);
    void jobInfoAnswered(QString jobPath, QVariantMap info);
    void overallProgressAnswered(double progress);

private:
    QStringList jobPaths;
    Manager* manager = nullptr;

    QHash<QString, JobCombo*> jobDict;  // job path to struct[Job, JobInfo]
    void updateJobDict();

    QStringList architectures;
    QStringList updatableApps;
    QSet<QString> installingAppsSet;
    QStringList installingApps;

    QSet<QString> runningJobsSet;
    QStringList runningJobs;

    double overallProgress = 0.0;

    void aggregateJobInfo();
    void onJobListChanged();
    void onUpdatableAppsChanged();
};


#endif //SHELL_LASTOREBRIDGE_H

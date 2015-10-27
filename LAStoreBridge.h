
#ifndef SHELL_LASTOREBRIDGE_H
#define SHELL_LASTOREBRIDGE_H

#include <lastore-daemon.h>
#include "ProgressButton.h"

using namespace dbus::common;
using namespace dbus::objects;
using namespace dbus::objects::com::deepin::lastore;


class LAStoreBridge : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList jobsInfo
               MEMBER jobsInfo)
    Q_PROPERTY(QStringList architectures
               MEMBER architectures)
    Q_PROPERTY(QStringList upgradableApps
               MEMBER upgradableApps)

public:
    explicit LAStoreBridge(QObject* parent = nullptr);
    ~LAStoreBridge();

public slots:
    void installApp(QString appId);
    void onJobListChanged();
    Q_SLOT void askAppInstalled(QString pkgId);
    QImage renderProgressButton(const int i);
    QImage renderOverallProgressButton();
    void launchApp(QString pkgId);
    Q_SLOT void askDownloadSize(QString pkgId);
    Q_SLOT void fetchUpgradableApps();
    Q_SLOT void startJob(QString jobId);
    Q_SLOT void pauseJob(QString jobId);
    Q_SLOT void cancelJob(QString jobId);

signals:
    void jobsInfoUpdated(); // let the webpage know there's update available.
    void progressButtonMouseEnter(int i);
    void progressButtonMouseLeave(int i);
    void upgradableAppsChanged();
    void appInstalledAnswered(QString pkgId, bool installed);
    void downloadSizeAnswered(QString pkgId, long long size);

private:
    Manager* manager = nullptr;
    QList<Job*> jobs;
    bool rebuildingJobs = false;
    QVariantList jobsInfo;
    ProgressButton* overallProgressButton = nullptr;
    QList<ProgressButton*> progressButtons;

    void onJobInfoUpdated(Job*); //
    void syncProgressButton(QVariant jobInfo, ProgressButton* button);

    void onProgressButtonMouseEnter(int i);
    void onProgressButtonMouseLeave(int i);

    QVariantMap processJob(Job* job);
    QVariantList processJobs(QList<Job *> list);
    QStringList architectures;
    QStringList upgradableApps;
};


#endif //SHELL_LASTOREBRIDGE_H

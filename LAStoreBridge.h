#ifndef SHELL_LASTOREBRIDGE_H
#define SHELL_LASTOREBRIDGE_H

#include <lastore-daemon.h>

using namespace dbus::common;
using namespace dbus::objects;
using namespace dbus::objects::com::deepin::lastore;

struct JobCombo {
    Job* object;
    QVariantMap info;
};

class LAStoreBridge : public QObject {
    Q_OBJECT
    Q_PROPERTY(QStringList jobPaths
               MEMBER jobPaths)
    Q_PROPERTY(QStringList updatableApps
               MEMBER updatableApps)
    Q_PROPERTY(QStringList installingApps
               MEMBER installingApps)

public:
    explicit LAStoreBridge(QObject* parent = nullptr);
    ~LAStoreBridge();

    void onJobListChanged();
public slots:
    Q_SLOT void installApp(const QString& appId);
    Q_SLOT void askSystemArchitectures();
    Q_SLOT void askAppInstalled(const QString& pkgId);
    Q_SLOT void launchApp(const QString& pkgId);
    Q_SLOT void askDownloadSize(const QString& pkgId);
    Q_SLOT void updateApp(const QString& appId);
    Q_SLOT void fetchUpdatableApps();
    Q_SLOT void startJob(const QString& jobId);
    Q_SLOT void pauseJob(const QString& jobId);
    Q_SLOT void cancelJob(const QString& jobId);
    Q_SLOT void askJobInfo(const QString& jobPath);
    Q_SLOT void askOverallProgress();

signals:
    void jobPathsChanged();

    void updatableAppsChanged();
    void systemArchitecturesAnswered(QStringList archs);
    void appInstalledAnswered(QString pkgId, bool installed);
    void downloadSizeAnswered(QString pkgId, long long size);
    void installingAppsChanged();
    void jobInfoAnswered(QVariantMap info);
    void overallProgressChanged(double progress);

private:
    QStringList jobPaths;
    Manager* manager = nullptr;

    QHash<QString, JobCombo*> jobDict;  // job path to struct[Job, JobInfo]
    void updateJobDict();

    QStringList architectures;
    QStringList updatableApps;
    QSet<QString> installingAppsSet;
    QStringList installingApps;
    double overallProgress = 0.0;

    void aggregateJobInfo();
};


#endif //SHELL_LASTOREBRIDGE_H

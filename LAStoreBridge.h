
#ifndef SHELL_LASTOREBRIDGE_H
#define SHELL_LASTOREBRIDGE_H

#include <lastore-daemon.h>
#include "ProgressButton.h"

using namespace dbus::common;
using namespace dbus::objects;
using namespace dbus::objects::org::deepin::lastore;


class LAStoreBridge : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList jobsInfo
               MEMBER jobsInfo)
    Q_PROPERTY(QStringList architectures
               MEMBER architectures)

public:
    explicit LAStoreBridge(QObject* parent = nullptr);
    ~LAStoreBridge();

public slots:
    void installApp(QString appId, QString region);
    void onJobListChanged();
    bool isAppInstalled(QString pkgId);
    QImage renderProgressButton(const int i);
    QImage renderOverallProgressButton();
    void launchApp(QString pkgId);
    long long getDownloadSize(QString pkgId);

signals:
    void jobsInfoUpdated(); // let the webpage know there's update available.
    void appInstallationStatusChanged(QString appId);
    void progressButtonMouseEnter(int i);
    void progressButtonMouseLeave(int i);
    void progressButtonsUpdated(int i);

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
};


#endif //SHELL_LASTOREBRIDGE_H


#include "LAStoreBridge.h"
#include "Bridge.h"
using namespace dbus::common;
using namespace dbus::objects;
using namespace dbus::objects::org::deepin::lastore;

LAStoreBridge::LAStoreBridge(QObject *parent) : QObject(parent) {
    this->manager = new Manager("system", "org.deepin.lastore", "/org/deepin/lastore", this);
    connect(this->manager, &Manager::jobListChanged,
            this, &LAStoreBridge::onJobListChanged);

    this->overallProgressButton = new ProgressButton();
    this->overallProgressButton->setBody(ProgressBody::None);
    this->overallProgressButton->resize(32, 32);

    connect(this, &LAStoreBridge::progressButtonMouseEnter,
            this, &LAStoreBridge::onProgressButtonMouseEnter,
            Qt::QueuedConnection);
    connect(this, &LAStoreBridge::progressButtonMouseLeave,
            this, &LAStoreBridge::onProgressButtonMouseLeave,
            Qt::QueuedConnection);

    this->onJobListChanged();

    this->architectures = this->manager->systemArchitectures().Value<0>();
}

LAStoreBridge::~LAStoreBridge() {
    if (this->manager) {
        delete this->manager;
        this->manager = nullptr;
    }
    if (this->overallProgressButton) {
        delete this->overallProgressButton;
        this->overallProgressButton = nullptr;
    }
}

void LAStoreBridge::installApp(QString appId, QString region) {
    R<QDBusObjectPath> rpath = this->manager->InstallPackage(appId, region);
    qDebug() << "installPackages returns path" << rpath.Value<0>().path();

    auto job = new Job("system", "org.deepin.lastore", rpath.Value<0>().path(), this);
    job->deleteLater();
}

void LAStoreBridge::onJobListChanged() {
    this->rebuildingJobs = true;
    auto jobList = this->manager->jobList();
    QList<QDBusObjectPath> pathsDBus = jobList.Value<0>();

    QList<QString> paths;
    std::transform(pathsDBus.constBegin(), pathsDBus.constEnd(), std::back_inserter(paths),
                   [](QDBusObjectPath path) {return path.path();});

    qDebug() << "job list changed" << paths;

    for (Job* job : this->jobs) {
        job->deleteLater();
        job = nullptr;
    }
    this->jobs = QList<Job*>();

    std::transform(paths.constBegin(), paths.constEnd(), std::back_inserter(this->jobs),
                   [this](QString path) {
                       auto job = new Job("system", "org.deepin.lastore", path, this);
                       auto notify = [job, this]() {
                           this->onJobInfoUpdated(job);
                       };
                       auto notifyInstallationStatusChanged = [job, this]() {
                           emit this->appInstallationStatusChanged(job->packageId().Value<0>());
                       };
                       connect(job, &Job::progressChanged, this, notify);
                       connect(job, &Job::elapsedTimeChanged, this, notify);
                       connect(job, &Job::statusChanged, this, notify);
                       connect(job, &Job::statusChanged, this, notifyInstallationStatusChanged);
                       return job;
                   });
    this->jobsInfo = this->processJobs(this->jobs);

    for (ProgressButton* btn : this->progressButtons) {
        btn->deleteLater();
        btn = nullptr;
    }
    this->progressButtons = QList<ProgressButton*>();

    std::transform(this->jobsInfo.constBegin(), this->jobsInfo.constEnd(), std::back_inserter(this->progressButtons),
                   [this](QVariant map) {
                       auto btn = new ProgressButton();
                       btn->resize(32, 32);
                       auto notify = [this]() {
                           emit this->jobsInfoUpdated();
                       };
                       connect(btn, &ProgressButton::needRepaint,
                               this, notify);
                       this->syncProgressButton(map, btn);
                       return btn;
                   });
    this->rebuildingJobs = false;
    emit this->jobsInfoUpdated();
}

void LAStoreBridge::onJobInfoUpdated(Job *job) {
    auto i = this->jobs.indexOf(job);
    this->jobsInfo[i] = this->processJob(job);
    this->syncProgressButton(this->jobsInfo[i], this->progressButtons[i]);
    emit this->jobsInfoUpdated();
}

QImage LAStoreBridge::renderProgressButton(const int i) {
    if (this->rebuildingJobs) {
        return QImage();
    }
    auto btn = this->progressButtons[i];
    QImage image(btn->size(), QImage::Format_ARGB32_Premultiplied);
    if (btn) {
        btn->render(&image);
    }
    return image;
}

void LAStoreBridge::syncProgressButton(QVariant jobInfo, ProgressButton* button) {
    auto map = jobInfo.value<QVariantMap>();
    double progress = map["progress"].value<double>();
    button->setBody(ProgressBody::Percentage);
    button->setProgress(progress);
    button->setState(map["status"].value<QString>());

    double sum = std::accumulate(this->jobsInfo.constBegin(),
                                 this->jobsInfo.constEnd(),
                                 0.0,
                                 [](const double a, QVariant jobInfo) {
                                     auto map = jobInfo.value<QVariantMap>();
                                     double progress = map["progress"].value<double>();
                                     return a + progress;
                                 });
    this->overallProgressButton->setProgress(sum / this->jobsInfo.size());
}

void LAStoreBridge::onProgressButtonMouseEnter(int i) {
    qDebug() << "onProgressButtonMouseEnter" << i;
    auto btn = this->progressButtons[i];
    if (btn) {
        QEvent enterEvent(QEvent::Enter);
        qApp->sendEvent(btn, &enterEvent);
    }
}

void LAStoreBridge::onProgressButtonMouseLeave(int i) {
    qDebug() << "onProgressButtonMouseLeave" << i;
    auto btn = this->progressButtons[i];
    if (btn) {
        QEvent leaveEvent(QEvent::Leave);
        qApp->sendEvent(btn, &leaveEvent);
    }
}

bool LAStoreBridge::isAppInstalled(QString pkgId) {
    auto reply = this->manager->PackageExists(pkgId);
    return reply.Value<0>();
}

QImage LAStoreBridge::renderOverallProgressButton() {
    auto btn = this->overallProgressButton;
    QImage image(btn->size(), QImage::Format_ARGB32_Premultiplied);
    if (btn) {
        btn->render(&image);
    }
    return image;
}

QVariantMap LAStoreBridge::processJob(Job* job) {
    QVariantMap each;
    auto progress = job->progress().Value<0>();
    each.insert("packageId", job->packageId().Value<0>());
    each.insert("type", job->type().Value<0>());
    each.insert("progress", progress);
    each.insert("elapsedTime", job->elapsedTime().Value<0>());
    QString status = job->status().Value<0>();
    each.insert("status", status);
    QString id = job->id().Value<0>();

    if (status == "ready") {
        qDebug() << "auto starts job";
        this->manager->StartJob(id);
    }

    return each;
}

QVariantList LAStoreBridge::processJobs(QList<Job *> list) {
    auto result = QVariantList();
    std::transform(list.constBegin(),
                   list.constEnd(),
                   std::back_inserter(result),
                   [this](Job* job) {
                       return this->processJob(job);
                   });
    return result;
}

void LAStoreBridge::launchApp(QString pkgId) {
    auto reply = this->manager->PackageDesktopPath1(pkgId);
    auto path = reply.Value<0>();
    auto bridge = static_cast<Bridge*>(this->parent());
    bridge->openDesktopFile(path);
}

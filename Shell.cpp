#include <QStandardPaths>
#include <QDir>
#include <QDBusMessage>
#include <QCommandLineParser>
#include <QSettings>
#include <QProcess>
#include "main.h"
#include "Shell.h"
#include "MainWindow.h"
#include "DBusInterface.h"
#include "ToolTip.h"


Shell::Shell(int &argc, char **argv) : QApplication(argc, argv) {
    this->parseOptions();
    this->basePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    this->settings = new QSettings(this);
    if (this->argsParser->isSet("clean")) {
        this->settings->clear();
        this->settings->sync();
        QDir baseDir(this->basePath);
        qDebug() << "clean done!";
        ::exit(!baseDir.removeRecursively());
    }

    try {
        this->dbusInterface = new DBusInterface(this);
    } catch (const char* name) {
        if (strcmp("ServiceExist", name) == 0) {
            const auto connection = QDBusConnection::sessionBus();
            const auto msg = QDBusMessage::createMethodCall("com.deepin.dstoreclient",
                                                            "/",
                                                            "com.deepin.dstoreclient",
                                                            "raise");
            connection.call(msg);
            qDebug() << "There is already a process running";
            ::exit(0);
        }
    }

    QObject::connect(this, &Shell::applicationCacheFinished,
                     this, &Shell::onApplicationCacheFinished);

    auto initUrl = this->argsParser->value("host");
    if (initUrl.size()) {
        this->initUrl = QUrl(initUrl);
    } else {
        this->initUrl = QUrl("http://appstore.deepin.org/");
    }

    this->origin = this->initUrl.scheme() + "://" + this->initUrl.host();

    this->settings->beginGroup(QString("cached"));
    const auto cached = this->settings->value(this->origin);
    this->settings->endGroup();
    this->isInitialRun = !cached.toString().size();
    ::restartQtLoop = this->isInitialRun;

    this->startWebView();
}

Shell::~Shell() {
    if (this->argsParser) {
        delete this->argsParser;
        this->argsParser = nullptr;
    }
    if (this->dbusInterface) {
        delete this->dbusInterface;
        this->dbusInterface = nullptr;
    }
}

void Shell::showTooltip(const QString& text, const QRect& globalGeometry) {
    if (this->tooltip) {
        delete this->tooltip;
        this->tooltip = nullptr;
    }
    if (text.isEmpty()) {
        return;
    }
    this->tooltip = new ToolTip();
    connect(this->tooltip, &ToolTip::destroyed, [this]() {
        this->tooltip = nullptr;
    });
    this->tooltip->show(text, globalGeometry);
}

void Shell::setTooltipVisible(const bool& visible) {
    if (!this->tooltip) {
        return;
    }
    this->tooltip->setVisible(visible);
}

void Shell::parseOptions() {
    this->argsParser = new QCommandLineParser();
    this->argsParser->setApplicationDescription("Deepin Store");
    this->argsParser->addHelpOption();
    this->argsParser->addVersionOption();

    this->argsParser->addOptions({
        {{"d", "debug"},
         QCoreApplication::translate("main", "Enable debug mode.")},
        {"host",
         QCoreApplication::translate("main", "Override the default host with the specified one."),
         QCoreApplication::translate("main", "host")},
        {{"c", "clean"},
         QCoreApplication::translate("main", "Remove deepin-store related files under $HOME.")},
        {{"o", "offline"},
         QCoreApplication::translate("main", "Start Deepin AppStore in offline mode (for testing purposes).")},
    });

    this->argsParser->process(qApp->arguments());
}

void Shell::onApplicationCacheFinished() {
    qDebug() << "onApplicationCacheFinished";
    if (this->isInitialRun) {
        this->settings->beginGroup("cached");
        this->settings->setValue(this->origin, true);
        this->settings->endGroup();
        this->settings->sync();
        this->isInitialRun = false;
        qDebug() << "Finish importing the initial webapp, restarting...";
        this->quit();
    }
}


void Shell::startWebView() {
    this->win = new MainWindow();
    this->win->setUrl(this->initUrl);
    if (!this->isInitialRun) {
        this->win->show();
        this->win->polish();
    }
}

void Shell::openManual() {
    QString program = "/usr/bin/dman";
    QStringList args;
    args << "deepin-appstore";
    QProcess::startDetached(program, args);
}

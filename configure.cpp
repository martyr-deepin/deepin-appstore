#include <QJsonDocument>
#include <QFile>
#include <QJsonObject>
#include <QSettings>
#include <QDebug>

#include "configure.h"

const char* defaultConfigurePath = "/etc/appstore.json";

QUrl autoSwitchHost() {
    QSettings s("/etc/deepin-version", QSettings::IniFormat, 0);
    s.setIniCodec("UTF-8");
    if (s.value("Release/Type").toString() == "Professional") {
        return QUrl("http://appstore.deepin.com/");
    }
    return QUrl("http://appstore.deepin.org/");
}

QString autoSwitchRegion() {
    QFile file("/etc/timezone");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open /etc/timezone for timezone information";
    }
    QString timezone = file.readAll().trimmed();

    if (timezone.isEmpty()) {
        qDebug() << "Error autoSwitchRegion got a empty timezone!";
    }

    if (timezone == "Asia/Shanghai" ||
        timezone == "Asia/Chongqing" ||
        timezone == "Asia/Chungking" ||
        timezone == "Asia/Urumqi" ||
        timezone == "Asia/Harbin" ||
        timezone == "Asia/PRC") {
        return "mainland";
    } else {
        return "international";
    }
}

const Configure LoadConfig()
{
    QFile file;
    file.setFileName(defaultConfigurePath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString txt = file.readAll();
    file.close();

    QJsonDocument d = QJsonDocument::fromJson(txt.toUtf8());
    QJsonObject cfg = d.object();

    QString region;
    QUrl host;

    if (cfg.contains("region")) {
        region = cfg.value("region").toString();
    } else {
        region = autoSwitchRegion();
    }

    if (cfg.contains("host")) {
        host = cfg.value("host").toString();
    } else {
        host = autoSwitchHost();
    }

    qDebug() << "Configure" << defaultConfigurePath << "parsed to" << host << region;
    return Configure{host, region};
}

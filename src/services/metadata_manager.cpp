/*
 * Copyright (C) 2018 Deepin Technology Co., Ltd.
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

#include "services/metadata_manager.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>

#include "base/file_util.h"
#include "services/settings_manager.h"
#include "services/backend/metadata_cache_worker.h"

namespace dstore
{

namespace
{

const char kMetadataAppList[] = "/api/app";
const char kOperationAppList[] = "/api/app";

QString GetCacheDir()
{
    const char kAppCacheDir[] = ".cache/deepin/deepin-appstore-metadata";
    return QDir::home().absoluteFilePath(kAppCacheDir);
}

}  // namespace

MetadataManager::MetadataManager(QObject *parent)
    : QObject(parent),
      cache_worker_(new MetadataCacheWorker(this)),
      cache_dir_(GetCacheDir()),
      metadata_server_(GetMetadataServer()),
      operation_server_(GetOperationServer()),
      apps_()
{
    this->setObjectName("MetadataManager");

    cache_dir_.mkpath(".");
    icon_dir_ = cache_dir_.absoluteFilePath("icons");
    icon_dir_.mkpath(".");
}

MetadataManager::~MetadataManager()
{
}

QString MetadataManager::getAppIcon(const QString &app_name)
{
    const QString filepath = icon_dir_.absoluteFilePath(app_name);
    if (QFileInfo::exists(filepath)) {
        return filepath;
    }

    if (apps_.isEmpty()) {
        if (!this->downloadMetadata()) {
            qCritical() << Q_FUNC_INFO << "Failed to download metadata";
            return QString();
        }
    }

    QString url;
    auto app = apps_.value(app_name);
    url = metadata_server_ + "/" + app.value("icon").toString();

    if (!url.isEmpty() && cache_worker_->downloadFile(url, filepath)) {
        return filepath;
    }

    return QString();
}

QString MetadataManager::getAppMetadataList(const QStringList &app_name_list)
{
    if (apps_.isEmpty()) {
        if (!this->downloadMetadata()) {
            qCritical() << Q_FUNC_INFO << "Failed to download metadata";
            return QString();
        }
    }

    QJsonObject apps;
    for (const auto &app : app_name_list) {
        auto obj = this->findMetadata(app);
        if (obj.isEmpty()) {
            continue;
        }
        obj["icon"] = getAppIcon(app);
        apps.insert(app, obj);

    }
    return QJsonDocument(apps).toJson();
}

bool isExpired(const QFileInfo &fi)
{
    return fi.lastModified().secsTo(QDateTime::currentDateTime()) > 3600 * 3;
}

bool MetadataManager::downloadMetadata()
{
    QMutexLocker locker(&mutex_);

    const QString index_file = cache_dir_.absoluteFilePath("index.json");
    QString index_url = operation_server_ + kOperationAppList;

    const QFileInfo index_file_info(index_file);
    if (!index_file_info.exists() || isExpired(index_file_info)) {
        if (!cache_worker_->downloadFile(index_url, index_file)) {
            qCritical() << Q_FUNC_INFO << "Failed to download app index file";
            return false;
        }
    }

    const QString metadata_file = cache_dir_.absoluteFilePath("metadata.json");
    QString metadata_url = metadata_server_ + kMetadataAppList;
    const QFileInfo metadata_file_info(metadata_file);
    if (!metadata_file_info.exists() || isExpired(metadata_file_info)) {
        if (!cache_worker_->downloadFile(metadata_url, metadata_file)) {
            qCritical() << Q_FUNC_INFO << "Failed to download metadata file";
            return false;
        }
    }

    // Now parse app index file and metadata file.
    return this->parseMetadata(index_file, metadata_file);
}

bool MetadataManager::parseMetadata(const QString &index_file,
                                    const QString &metadata_file)
{
    apps_.clear();
    QByteArray index_content;
    if (!ReadRawFile(index_file, index_content)) {
        qCritical() << Q_FUNC_INFO << "failed to read index content";
        return false;
    }

    QByteArray metadata_content;
    if (!ReadRawFile(metadata_file, metadata_content)) {
        qCritical() << Q_FUNC_INFO << "failed to read metadata content";
        return false;
    }

    QStringList index_list;
    QJsonDocument index_doc(QJsonDocument::fromJson(index_content));
    if (index_doc.isObject()) {
        QJsonObject obj = index_doc.object();
        QJsonValue apps_val = obj.value("apps");
        QJsonArray apps_arr = apps_val.toArray();
        for (const QJsonValue &app : apps_arr) {
            index_list.append(app.toString());
        }
    } else {
        return false;
    }

    QJsonDocument metadata_doc(QJsonDocument::fromJson(metadata_content));
    if (metadata_doc.isObject()) {
        QJsonObject obj = metadata_doc.object();
        QJsonValue apps_val = obj.value("apps");
        QJsonArray apps_arr = apps_val.toArray();
        for (const QJsonValue &app : apps_arr) {
            const QJsonObject app_obj = app.toObject();
            const QString name = app_obj.value("name").toString();

            if (!index_list.contains(name)) {
                continue;
            }

            const QString icon = app_obj.value("icon").toString();
            const QString category = app_obj.value("category").toString();
//            apps_.append(AppMetadata{name, icon, category});
            apps_.insert(name, app.toObject());
        }
    } else {
        return false;
    }

    return true;
}

QJsonObject MetadataManager::findMetadata(const QString &app_name)
{
    // FIXME(Shaohua): Convert package name
    return apps_.value(app_name);
}

void MetadataManager::downloadAppIcons()
{
    if (apps_.isEmpty()) {
        if (!this->downloadMetadata()) {
            qCritical() << Q_FUNC_INFO << "Failed to download metadata";
            return;
        }
    }

    for (const auto &app : apps_) {
        const QString url = metadata_server_ + "/" + app.value("icon").toString();
        const QString filepath = icon_dir_.absoluteFilePath(app.value("name").toString());
        if (!QFileInfo::exists(filepath) && !url.isEmpty()) {
            cache_worker_->downloadFile(url, filepath);
        }
    }
}

}  // namespace dstore

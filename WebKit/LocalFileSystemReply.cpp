#include <QtGlobal>
#include <QFile>
#include <QTimer>
#include "LocalFileSystemReply.h"


LocalFileSystemReply::LocalFileSystemReply(QNetworkAccessManager::Operation op,
                                           const QNetworkRequest& req,
                                           QObject* parent,
                                           QIODevice* outgoingData) : QNetworkReply(parent) {
    auto urlStr = req.url().path();
    if (urlStr == "/") {
        urlStr = "/index.html";
    }
    req.url().setPath(urlStr);
    this->setUrl(req.url());

    QFile file("/usr/share/deepin-appstore/webapp" + urlStr);
    file.open(ReadOnly | Unbuffered);
    this->content = file.readAll();
    file.close();

    this->open(ReadOnly | Unbuffered);
    if (urlStr.endsWith(".html")) {
        this->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/html"));
    } else if (urlStr.endsWith(".appcache")) {
        this->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/cache-manifest"));
    } else if (urlStr.endsWith(".js")) {
        this->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/javascript"));
    } else if (urlStr.endsWith(".css")) {
        this->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/css"));
    } else if (urlStr.endsWith(".svg")) {
        this->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/svg+xml"));
    } else if (urlStr.endsWith(".gif")) {
        this->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/gif"));
    } else if (urlStr.endsWith(".png")) {
        this->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/png"));
    } else if (urlStr.endsWith(".ico")) {
        this->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/x-icon"));
    } else if (urlStr.endsWith(".otf")) {
        this->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/font-sfnt"));
    } else {
        qWarning() << "ContentType didn't specify for " << urlStr;
    }

    this->setHeader(QNetworkRequest::ContentLengthHeader, QVariant(content.size()));
    this->setOperation(QNetworkAccessManager::Operation::GetOperation);
    this->setAttribute(QNetworkRequest::HttpStatusCodeAttribute, 200);

    this->timer = new QTimer(this);
    QObject::connect(this->timer, &QTimer::timeout, [this]() {
        emit this->finished();
    });
    this->timer->setSingleShot(true);
    this->timer->start(0);
}

LocalFileSystemReply::~LocalFileSystemReply() {
    if (this->timer) {
        this->timer->stop();
        delete this->timer;
        this->timer = nullptr;
    }
}

qint64 LocalFileSystemReply::readData(char *data, qint64 maxlen) {
    if (this->offset >= this->content.size())
        return -1;

    qint64 number = qMin(maxlen, this->content.size() - this->offset);
    memcpy(data, this->content.constData() + this->offset, number);
    this->offset += number;

    return number;
}

qint64 LocalFileSystemReply::bytesAvailable() const {
    return this->content.size() - this->offset;
}


qint64 LocalFileSystemReply::writeData(const char *data, qint64 len) {
    throw "Not Implemented(writeData)";
}

void LocalFileSystemReply::close() {
    throw "Not Implemented(close)";
}

void LocalFileSystemReply::abort() {

}

bool LocalFileSystemReply::isSequential() const {
    return true;
}

void LocalFileSystemReply::sslConfigurationImplementation(QSslConfiguration &configuration) const {
    qWarning() << "sslConfigurationImplementation";
    throw "Not Implemented";
}

void LocalFileSystemReply::setSslConfigurationImplementation(const QSslConfiguration &configuration) {
    qWarning() << "setSslConfigurationImplementation";
    throw "Not Implemented";
}

void LocalFileSystemReply::ignoreSslErrorsImplementation(const QList<QSslError> &list) {
    qWarning() << "ignoreSslErrorsImplementation";
    throw "Not Implemented";
}

void LocalFileSystemReply::ignoreSslErrors() {
    qWarning() << "ignoreSslErrors";
    throw "Not Implemented";
}

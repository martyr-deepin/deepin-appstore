#ifndef SHELL_LOCALFILESYSTEMREPLY_H
#define SHELL_LOCALFILESYSTEMREPLY_H

#include <QNetworkReply>

class LocalFileSystemReply : public QNetworkReply {
    Q_OBJECT
public:
    explicit LocalFileSystemReply(QNetworkAccessManager::Operation op,
                                  const QNetworkRequest &req,
                                  QObject* parent = nullptr,
                                  QIODevice *outgoingData = nullptr);
    ~LocalFileSystemReply();


    void close() Q_DECL_OVERRIDE;
    bool isSequential() const Q_DECL_OVERRIDE;
    qint64 bytesAvailable() const Q_DECL_OVERRIDE;

public Q_SLOTS:
    void abort() Q_DECL_OVERRIDE;
    void ignoreSslErrors() Q_DECL_OVERRIDE;

protected:
    qint64 readData(char *data, qint64 maxlen) Q_DECL_OVERRIDE;
    qint64 writeData(const char *data, qint64 len) Q_DECL_OVERRIDE;
    void sslConfigurationImplementation(QSslConfiguration &) const Q_DECL_OVERRIDE;
    void setSslConfigurationImplementation(const QSslConfiguration &) Q_DECL_OVERRIDE;
    void ignoreSslErrorsImplementation(const QList<QSslError> &) Q_DECL_OVERRIDE;

private:
    QByteArray content;
    qint64 offset = 0;

    QTimer* timer = nullptr;
};


#endif //SHELL_LOCALFILESYSTEMREPLY_H

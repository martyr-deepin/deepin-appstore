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
    qint64 bytesAvailable() const;

public Q_SLOTS:
    void abort();
    void ignoreSslErrors();

protected:
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len) Q_DECL_OVERRIDE;
    void sslConfigurationImplementation(QSslConfiguration &) const;
    void setSslConfigurationImplementation(const QSslConfiguration &);
    void ignoreSslErrorsImplementation(const QList<QSslError> &);

private:
    QByteArray content;
    qint64 offset = 0;

    QTimer* timer = nullptr;
};


#endif //SHELL_LOCALFILESYSTEMREPLY_H

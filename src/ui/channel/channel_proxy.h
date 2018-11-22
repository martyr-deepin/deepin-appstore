
#include <QObject>
#include <QWebChannelAbstractTransport>
#include <QJsonDocument>
#include <QJsonObject>

namespace dstore {

/**
 * This proxy object is used by web page to write log messages to local
 * log file.
 */

class ChannelTransport : public QWebChannelAbstractTransport{
  Q_OBJECT
  public:
    ChannelTransport(QObject *parent): QWebChannelAbstractTransport(parent) {
    }
    signals:
      void sendMessageString(const QString &msg);
    public slots:
      void sendMessage(const QJsonObject &msg){
        QJsonDocument doc(msg);
        emit this->sendMessageString(doc.toJson());
      }
};

class ChannelProxy : public QObject {
  Q_OBJECT
 public:
  ChannelTransport *transport = new ChannelTransport(this);
  ChannelProxy(QObject *parent): QObject(parent) {
    connect(this->transport,&ChannelTransport::sendMessageString,this,&ChannelProxy::message);
  }
 signals:
  void message(const QString &msgData);
 public slots:
  void send(const QString &msgData){
    auto doc = QJsonDocument::fromJson(msgData.toUtf8());
    if( doc.isObject() ){
      emit this->transport->messageReceived(doc.object(), this->transport);
    }
  }
};

}  // namespace dstore

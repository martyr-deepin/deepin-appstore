#pragma once

#include <QObject>
#include <QVariantMap>

namespace dstore
{
class AccountManager;
class AccountProxy : public QObject
{
    Q_OBJECT
public:
    explicit AccountProxy(QObject *parent = Q_NULLPTR);

Q_SIGNALS:
    void userInfoChanged(const QVariantMap &info);

public Q_SLOTS:
    QVariantMap getUserInfo() const;

    QString getToken() const;

    void login();

    void logout();

private:
    AccountManager *manager_;
};

} // namespace dstore

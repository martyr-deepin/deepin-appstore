#include "account_proxy.h"

#include "services/account_manager.h"

namespace dstore
{

AccountProxy::AccountProxy(QObject *parent) : QObject(parent)
{
    manager_ = new AccountManager(parent);
    connect(manager_, &AccountManager::userInfoChanged,
            this, &AccountProxy::userInfoChanged);
}

QVariantMap dstore::AccountProxy::getUserInfo() const
{
    return manager_->getUserInfo();
}

QString dstore::AccountProxy::getToken() const
{
    return manager_->getToken();
}

void dstore::AccountProxy::login()
{
    manager_->login();
}

void dstore::AccountProxy::logout()
{
    manager_->logout();
}

} // namespace dstore

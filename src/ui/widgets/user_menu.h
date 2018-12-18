#pragma once

#include <QMenu>
#include <QScopedPointer>

namespace dstore
{

class UserMenuPrivate;
class UserMenu : public QMenu
{
    Q_OBJECT
public:
    explicit UserMenu(QWidget *parent = Q_NULLPTR);
    ~UserMenu();

Q_SIGNALS:
    void requestLogout();
    void commentRequested();
    void requestDonates();
    void requestApps();

public Q_SLOTS:
    void setUsername(const QString &username);

private:
    QScopedPointer<UserMenuPrivate> dd_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(dd_ptr), UserMenu)
};

}

#include "user_menu.h"

#include <QAction>

namespace dstore
{

class UserMenuPrivate
{
public:
    UserMenuPrivate(UserMenu *parent) : q_ptr(parent)
    {
        userName = parent->addAction(UserMenu::tr("Username"));
        userName->setDisabled(true);

        comment = parent->addAction(UserMenu::tr("My comments"));
        parent->connect(comment, &QAction::triggered,
                        parent, &UserMenu::commentRequested);

        reward = parent->addAction(UserMenu::tr("My reward"));
        parent->connect(reward, &QAction::triggered,
                        parent, &UserMenu::requestReward);

        apps = parent->addAction(UserMenu::tr("My apps"));
        parent->connect(apps, &QAction::triggered,
                        parent, &UserMenu::requestApps);

        parent->addSeparator();

        logout = parent->addAction(UserMenu::tr("Logout"));
        parent->connect(logout, &QAction::triggered,
                        parent, &UserMenu::requestLogout);
    }

    QAction *userName;
    QAction *comment;
    QAction *reward;
    QAction *apps;
    QAction *logout;

    UserMenu *q_ptr;
    Q_DECLARE_PUBLIC(UserMenu)
};

UserMenu::UserMenu(QWidget *parent) :
    QMenu(parent), dd_ptr(new UserMenuPrivate(this))
{
}

UserMenu::~UserMenu()
{

}

void UserMenu::setUsername(const QString &username)
{
    Q_D(UserMenu);
    d->userName->setText(username);
}

}

#include "user_menu.h"

#include <QAction>

namespace dstore
{

class UserMenuPrivate
{
public:
    UserMenuPrivate(UserMenu *parent) : q_ptr(parent)
    {
        userName = new QAction(UserMenu::tr("Username"));
        userName->setDisabled(true);
        parent->addAction(userName);

        comment = new QAction(UserMenu::tr("My comments"));
        parent->connect(comment, &QAction::triggered,
                        parent, &UserMenu::commentRequested);
        parent->addAction(comment);

        reward = new QAction(UserMenu::tr("My reward"));
        parent->connect(reward, &QAction::triggered,
                        parent, &UserMenu::requestReward);
        parent->addAction(reward);

        apps = new QAction(UserMenu::tr("My apps"));
        parent->connect(apps, &QAction::triggered,
                        parent, &UserMenu::requestApps);
        parent->addAction(apps);

        parent->addSeparator();

        logout = new QAction(UserMenu::tr("Logout"));
        parent->connect(logout, &QAction::triggered,
                        parent, &UserMenu::requestLogout);
        parent->addAction(logout);
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

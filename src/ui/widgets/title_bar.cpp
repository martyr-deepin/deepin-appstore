/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
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

#include "ui/widgets/title_bar.h"

#include <QDebug>
#include <QTimer>
#include <QDir>
#include <QPainter>
#include <QJsonObject>
#include <DThemeManager>

#include "ui/widgets/search_edit.h"
#include "ui/widgets/user_menu.h"
#include "base/consts.h"

namespace dstore
{

TitleBar::TitleBar(QWidget *parent) : QFrame(parent)
{
    this->setObjectName("TitleBar");
    this->initUI();
    this->initConnections();
}

TitleBar::~TitleBar()
{
}

QString TitleBar::getSearchText() const
{
    QString text = search_edit_->text();
    return text.remove('\n').remove('\r').remove("\r\n");
}

void TitleBar::setBackwardButtonActive(bool active)
{
    back_button_->setEnabled(active);
}

void TitleBar::setForwardButtonActive(bool active)
{
    forward_button_->setEnabled(active);
}

void TitleBar::setUserInfo(const QJsonObject &info)
{
    user_name_ = info.value("name").toString();
    user_menu_->setUsername(user_name_);
    if (user_name_.isEmpty()) {
        avatar_button_->setObjectName("AvatarButton");
        avatar_button_->setStyleSheet(this->styleSheet());
        return;
    }

    QDir cache_dir(dstore::GetCacheDir());
    auto avatarPath = cache_dir.filePath("avatar.png");

    avatar_button_->setObjectName("AvatarButtonUser");
    auto base64Data = QByteArray::fromStdString(info.value("profile_image").toString().toStdString());
    auto imageData = QByteArray::fromBase64(base64Data);
    auto image = QImage::fromData(imageData);
    saveUserAvatar(image, avatarPath);
    auto style = QString("#AvatarButtonUser {border-image: url(%1);}").arg(avatarPath);
    avatar_button_->setStyleSheet(style);
}

void TitleBar::saveUserAvatar(const QImage &image, const QString &filePath)
{
    QSize sz = avatar_button_->size();

    QPixmap maskPixmap(sz);
    maskPixmap.fill(Qt::transparent);
    QPainterPath path;
    path.addEllipse(QRectF(0, 0, sz.width(), sz.height()));
    QPainter maskPainter(&maskPixmap);
    maskPainter.setRenderHint(QPainter::Antialiasing);
    maskPainter.setPen(QPen(Qt::white, 1));
    maskPainter.fillPath(path, QBrush(Qt::white));

    QPainter::CompositionMode mode = QPainter::CompositionMode_SourceIn;
    QImage contentImage = QImage(sz, QImage::Format_ARGB32_Premultiplied);
    QPainter contentPainter(&contentImage);
    contentPainter.setCompositionMode(QPainter::CompositionMode_Source);
    contentPainter.fillRect(contentImage.rect(), Qt::transparent);
    contentPainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    contentPainter.drawImage(0, 0, maskPixmap.toImage());
    contentPainter.setCompositionMode(mode);
    contentPainter.drawImage(0, 0, image.scaled(avatar_button_->size(),
                             Qt::KeepAspectRatio,
                             Qt::SmoothTransformation));
    contentPainter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    contentPainter.end();
    contentImage = contentImage.scaled(avatar_button_->size());
    contentImage.save(filePath);

    QPixmap pixmap = QPixmap::fromImage(contentImage);

//    QPalette palette;
//    palette.setBrush(avatar_button_->backgroundRole(),
//                     QBrush(pixmap));

    avatar_button_->setFlat(true);
    avatar_button_->setAutoFillBackground(true);
//    avatar_button_->setPalette(palette);
}

void TitleBar::initConnections()
{
    connect(back_button_, &Dtk::Widget::DImageButton::clicked,
            this, &TitleBar::backwardButtonClicked);
    connect(forward_button_, &Dtk::Widget::DImageButton::clicked,
            this, &TitleBar::forwardButtonClicked);
    connect(search_edit_, &SearchEdit::textChanged,
            this, &TitleBar::onSearchTextChanged);
    connect(search_edit_, &SearchEdit::focusOut,
            this, &TitleBar::focusOut);
    connect(search_edit_, &SearchEdit::downKeyPressed,
            this, &TitleBar::downKeyPressed);
    connect(search_edit_, &SearchEdit::enterPressed,
            this, &TitleBar::enterPressed);
    connect(search_edit_, &SearchEdit::upKeyPressed,
            this, &TitleBar::upKeyPressed);

    connect(avatar_button_, &QPushButton::clicked,
    this, [&]() {
        if (user_name_.isEmpty()) {
            Q_EMIT loginRequested(true);
        } else {
            auto x = avatar_button_->rect().left();
            auto y = avatar_button_->rect().bottom() + 10;
            user_menu_->popup(avatar_button_->mapToGlobal(QPoint(x, y)));
        }
    });

    connect(user_menu_, &UserMenu::requestLogout,
    this, [&] {
        Q_EMIT this->loginRequested(false);
    });
    connect(user_menu_, &UserMenu::commentRequested,
            this, &TitleBar::commentRequested);
    connect(user_menu_, &UserMenu::requestReward,
            this, &TitleBar::requestReward);
    connect(user_menu_, &UserMenu::requestApps,
            this, &TitleBar::requestApps);
}

void TitleBar::initUI()
{
    QLabel *app_icon = new QLabel();
    app_icon->setObjectName("AppIcon");
    app_icon->setFixedSize(26, 26);

    back_button_ = new Dtk::Widget::DImageButton();
    back_button_->setObjectName("BackButton");
    back_button_->setFixedSize(26, 26);

    forward_button_ = new Dtk::Widget::DImageButton();
    forward_button_->setObjectName("ForwardButton");
    forward_button_->setFixedSize(26, 26);

    avatar_button_ = new QPushButton();
    avatar_button_->setObjectName("AvatarButton");
    avatar_button_->setFixedSize(20, 20);
    avatar_button_->setContextMenuPolicy(Qt::CustomContextMenu);

    user_menu_ = new UserMenu();

    QHBoxLayout *left_layout = new QHBoxLayout();
    left_layout->setSpacing(0);
    left_layout->setContentsMargins(0, 0, 0, 0);
    left_layout->addWidget(app_icon);
    left_layout->addSpacing(10);
    left_layout->addWidget(back_button_);
    left_layout->addWidget(forward_button_);
    left_layout->addStretch();

    QFrame *left_buttons = new QFrame();
    left_buttons->setFixedWidth(26 + 10 + 26 + 26);
    left_buttons->setContentsMargins(0, 0, 0, 0);
    left_buttons->setLayout(left_layout);

    search_edit_ = new SearchEdit();
    search_edit_->setObjectName("SearchEdit");
    search_edit_->setFixedSize(242, 26);
    search_edit_->setPlaceHolder(QObject::tr("Search"));

    QHBoxLayout *main_layout = new QHBoxLayout();
    main_layout->setSpacing(0);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->addWidget(left_buttons);
    main_layout->addSpacing(160);
    main_layout->addWidget(search_edit_, 1, Qt::AlignCenter);
    main_layout->addWidget(avatar_button_, 0, Qt::AlignVCenter | Qt::AlignRight);
    main_layout->addSpacing(170);
    this->setLayout(main_layout);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    Dtk::Widget::DThemeManager::instance()->registerWidget(this);
}

void TitleBar::onSearchTextChanged()
{
    emit this->searchTextChanged(search_edit_->text());
}

}  // namespace dstore

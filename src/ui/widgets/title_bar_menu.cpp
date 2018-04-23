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

#include "ui/widgets/title_bar_menu.h"

#include <QDebug>

namespace dstore {

TitleBarMenu::TitleBarMenu(bool support_sign_in, QWidget* parent)
    : QMenu(parent),
      support_sign_in_(support_sign_in) {

  this->initActions();
}

TitleBarMenu::~TitleBarMenu() {

}

bool TitleBarMenu::isLoggedIn() const {
  return is_signed_in_;
}

bool TitleBarMenu::getRegion() const {
  return region_group_->checkedAction() == region_china_;
}

bool TitleBarMenu::isDarkTheme() const {
  return is_dark_theme_;
}

void TitleBarMenu::setLoginState(bool login) {
  Q_ASSERT(support_sign_in_);

  is_signed_in_ = login;
  if (support_sign_in_) {
    if (login) {
      sign_in_action_->setText(QObject::tr("Sign Out"));
    } else {
      sign_in_action_->setText(QObject::tr("Sign In"));
    }
  }
}

void TitleBarMenu::setRegion(bool is_china) {
  if (is_china) {
    region_china_->setChecked(true);
  } else {
    region_international_->setChecked(true);
  }
}

void TitleBarMenu::setDarkTheme(bool is_dark_theme) {
  is_dark_theme_ = is_dark_theme;
  if (is_dark_theme) {
    switch_theme_action_->setText(QObject::tr("Light Theme"));
  } else {
    switch_theme_action_->setText(QObject::tr("Dark Theme"));
  }
}

void TitleBarMenu::initActions() {
  if (support_sign_in_) {
    sign_in_action_ = this->addAction(QObject::tr("Sign In"));
    connect(sign_in_action_, &QAction::triggered,
            this, &TitleBarMenu::onSignInActionTriggered);

    this->addAction(QObject::tr("Recommend App"),
                    this, &TitleBarMenu::recommendAppRequested);
  }

  auto region_menu = this->addMenu(QObject::tr("Select Region"));
  region_china_ = region_menu->addAction(QObject::tr("China"));
  region_china_->setCheckable(true);
  region_international_ = region_menu->addAction(QObject::tr("International"));
  region_international_->setCheckable(true);
  region_group_ = new QActionGroup(this);
  region_group_->setExclusive(true);
  region_group_->addAction(region_china_);
  region_group_->addAction(region_international_);
  region_china_->setChecked(true);
  connect(region_group_, &QActionGroup::triggered,
          this, &TitleBarMenu::onRegionGroupTriggered);

  this->addAction(QObject::tr("Clear Cache"),
                  this, &TitleBarMenu::clearCacheRequested);

  switch_theme_action_ = this->addAction(QObject::tr("Dark Theme"));
  connect(switch_theme_action_, &QAction::triggered,
          this, &TitleBarMenu::onThemeActionTriggered);

  this->addSeparator();
}

void TitleBarMenu::onSignInActionTriggered() {
  emit this->loginRequested(!is_signed_in_);
}

void TitleBarMenu::onThemeActionTriggered() {
  emit this->switchThemeRequested(!is_dark_theme_);
}

void TitleBarMenu::onRegionGroupTriggered(QAction* action) {
  emit this->switchRegionRequested(action == region_china_);
}

}  // namespace dstore
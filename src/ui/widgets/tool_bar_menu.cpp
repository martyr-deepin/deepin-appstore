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

#include "ui/widgets/tool_bar_menu.h"

#include <QDebug>

namespace dstore {

ToolBarMenu::ToolBarMenu(bool support_sign_in, QWidget* parent)
    : QMenu(parent),
      support_sign_in_(support_sign_in) {

  this->initActions();
}

ToolBarMenu::~ToolBarMenu() {

}

bool ToolBarMenu::isSignedIn() const {
  return is_signed_in_;
}

bool ToolBarMenu::getRegion() const {
  return region_group_->checkedAction() == region_china_;
}

bool ToolBarMenu::isDarkTheme() const {
  return is_dark_theme_;
}

void ToolBarMenu::setSignedIn(bool is_signed_in) {
  Q_ASSERT(support_sign_in_);

  is_signed_in_ = is_signed_in;
  if (support_sign_in_) {
    if (is_signed_in) {
      sign_in_action_->setText(QObject::tr("Sign Out"));
    } else {
      sign_in_action_->setText(QObject::tr("Sign In"));
    }
  }
}

void ToolBarMenu::setRegion(bool is_china) {
  if (is_china) {
    region_china_->setChecked(true);
  } else {
    region_international_->setChecked(true);
  }
}

void ToolBarMenu::setDarkTheme(bool is_dark_theme) {
  is_dark_theme_ = is_dark_theme;
  if (is_dark_theme) {
    switch_theme_action_->setText(QObject::tr("Light Theme"));
  } else {
    switch_theme_action_->setText(QObject::tr("Dark Theme"));
  }
}

void ToolBarMenu::initActions() {
  if (support_sign_in_) {
    sign_in_action_ = this->addAction(QObject::tr("Sign In"));
    connect(sign_in_action_, &QAction::triggered,
            this, &ToolBarMenu::onSignInActionTriggered);

    this->addAction(QObject::tr("Recommend App"),
                    this, &ToolBarMenu::recommendAppRequested);
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
          this, &ToolBarMenu::onRegionGroupTriggered);

  this->addAction(QObject::tr("Clear Cache"),
                  this, &ToolBarMenu::clearCacheRequested);

  switch_theme_action_ = this->addAction(QObject::tr("Dark Theme"));
  connect(switch_theme_action_, &QAction::triggered,
          this, &ToolBarMenu::onThemeActionTriggered);

  this->addSeparator();
}

void ToolBarMenu::onSignInActionTriggered() {
  this->setSignedIn(!is_signed_in_);
}

void ToolBarMenu::onThemeActionTriggered() {
  this->setDarkTheme(!is_dark_theme_);
}

void ToolBarMenu::onRegionGroupTriggered(QAction* action) {
  emit this->regionChanged(action == region_china_);
}

}  // namespace dstore
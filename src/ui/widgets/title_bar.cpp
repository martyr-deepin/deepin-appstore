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

#include "ui/utils/theme_manager.h"
#include "ui/widgets/search_edit.h"

namespace dstore {

TitleBar::TitleBar(QWidget* parent) : QFrame(parent) {
  this->setObjectName("TitleBar");
  this->initUI();
  this->initConnections();
}

TitleBar::~TitleBar() {
}

QString TitleBar::getSearchText() const {
  QString text = search_edit_->text();
  return text.remove('\n').remove('\r').remove("\r\n");
}

void TitleBar::setBackwardButtonActive(bool active) {
  back_button_->setEnabled(active);
}

void TitleBar::setForwardButtonActive(bool active) {
  forward_button_->setEnabled(active);
}

void TitleBar::initConnections() {
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
}

void TitleBar::initUI() {
  QLabel* app_icon = new QLabel();
  app_icon->setObjectName("AppIcon");
  app_icon->setFixedSize(26, 26);

  back_button_ = new Dtk::Widget::DImageButton();
  back_button_->setObjectName("BackButton");
  back_button_->setFixedSize(26, 26);

  forward_button_ = new Dtk::Widget::DImageButton();
  forward_button_->setObjectName("ForwardButton");
  forward_button_->setFixedSize(26, 26);

  QHBoxLayout* left_layout = new QHBoxLayout();
  left_layout->setSpacing(0);
  left_layout->setContentsMargins(0, 0, 0, 0);
  left_layout->addWidget(app_icon);
  left_layout->addSpacing(10);
  left_layout->addWidget(back_button_);
  left_layout->addWidget(forward_button_);
  left_layout->addStretch();
  QFrame* left_buttons = new QFrame();
  left_buttons->setFixedWidth(26 + 10 + 26 + 26);
  left_buttons->setContentsMargins(0, 0, 0, 0);
  left_buttons->setLayout(left_layout);

  search_edit_ = new SearchEdit();
  search_edit_->setObjectName("SearchEdit");
  search_edit_->setFixedSize(242, 26);
  search_edit_->setPlaceHolder(QObject::tr("Search"));

  QHBoxLayout* main_layout = new QHBoxLayout();
  main_layout->setSpacing(0);
  main_layout->setContentsMargins(0, 0, 0, 0);
  main_layout->addWidget(left_buttons);
  main_layout->addWidget(search_edit_, 1, Qt::AlignCenter);
  main_layout->addSpacing(48);
  this->setLayout(main_layout);

  this->setAttribute(Qt::WA_TranslucentBackground, true);

  ThemeManager::instance()->registerWidget(this);
}

void TitleBar::onSearchTextChanged() {
  emit this->searchTextChanged(search_edit_->text());
}

}  // namespace dstore

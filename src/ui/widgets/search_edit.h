/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef DEEPIN_APPSTORE_UI_WIDGETS_SEARCH_EDIT_H
#define DEEPIN_APPSTORE_UI_WIDGETS_SEARCH_EDIT_H

#include <DSearchEdit>

namespace dstore {

/**
 * Provides an edit box in TitleBar.
 */
class SearchEdit : public Dtk::Widget::DSearchEdit {
  Q_OBJECT
 public:
  explicit SearchEdit(QWidget* parent = nullptr);
  ~SearchEdit() override;

 signals:
  void downKeyPressed();
  void enterPressed();
  void upKeyPressed();

 protected:
  void keyPressEvent(QKeyEvent* event) override;
};

}  // namespace dstore

#endif  // DEEPIN_APPSTORE_UI_WIDGETS_SEARCH_EDIT_H

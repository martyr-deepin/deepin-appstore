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

#ifndef DEEPIN_APPSTORE_UI_WIDGETS_RECOMMEND_APP_H
#define DEEPIN_APPSTORE_UI_WIDGETS_RECOMMEND_APP_H

#include <dlineedit.h>
#include <dabstractdialog.h>
#include <QComboBox>
#include <QPushButton>

namespace dstore {

/**
 * Displays recommend app form
 */
class RecommendApp : public QDialog {
  Q_OBJECT
 public:
  explicit RecommendApp(QWidget* parent = nullptr);
  ~RecommendApp() override;

 signals:
  void submit();

 public slots:
  void clearForm();

 private:
  void initConnections();
  void initUI();

  Dtk::Widget::DLineEdit* app_name_ = nullptr;
  QComboBox* app_type_ = nullptr;
  Dtk::Widget::DLineEdit* license_type_ = nullptr;
  Dtk::Widget::DLineEdit* home_page_ = nullptr;
  Dtk::Widget::DLineEdit* download_addr_ = nullptr;
  Dtk::Widget::DLineEdit* misc_ = nullptr;
  QPushButton* submit_ = nullptr;
};

}  // namespace store

#endif  // DEEPIN_APPSTORE_UI_WIDGETS_RECOMMEND_APP_H

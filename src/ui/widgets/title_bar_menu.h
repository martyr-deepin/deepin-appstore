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

#ifndef DEEPIN_APPSTORE_UI_WIDGETS_TOOL_BAR_MENU_H
#define DEEPIN_APPSTORE_UI_WIDGETS_TOOL_BAR_MENU_H

#include <QMenu>

namespace dstore {

class TitleBarMenu : public QMenu {
  Q_OBJECT
  Q_PROPERTY(bool loginState
                 READ isLoggedIn
                 WRITE setLoginState
                 NOTIFY loginRequested)
  Q_PROPERTY(bool darkTheme
                 READ isDarkTheme
                 WRITE setDarkTheme
                 NOTIFY switchThemeRequested)

 public:
  explicit TitleBarMenu(bool support_sign_in, QWidget* parent = nullptr);
  ~TitleBarMenu() override;

  bool isLoggedIn() const;

  bool isDarkTheme() const;

 signals:
  void loginRequested(bool login);
  void switchThemeRequested(bool is_dark_theme);
  void recommendAppRequested();
  void regionChanged();
  void clearCacheRequested();

 public slots:
  void setLoginState(bool login);
  void setRegion(bool is_china);
  void setDarkTheme(bool is_dark_theme);

 private:
  void initActions();

  bool support_sign_in_ = false;

  bool is_signed_in_ = false;
  QAction* sign_in_action_ = nullptr;
  QActionGroup* region_group_ = nullptr;
  QAction* region_china_ = nullptr;
  QAction* region_international_ = nullptr;
  bool is_dark_theme_ = false;
  QAction* switch_theme_action_ = nullptr;

 private slots:
  void onSignInActionTriggered();
  void onThemeActionTriggered();
  void onRegionGroupTriggered(QAction* action);
};

}  // namespace dman

#endif // DEEPIN_APPSTORE_UI_WIDGETS_TOOL_BAR_MENU_H

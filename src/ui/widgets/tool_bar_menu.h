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

#ifndef DEEPIN_APPSTORE_UI_WIDGETS_TOOL_BAR_MENU_H
#define DEEPIN_APPSTORE_UI_WIDGETS_TOOL_BAR_MENU_H

#include <QMenu>

namespace dstore {

class ToolBarMenu : public QMenu {
  Q_OBJECT
  Q_PROPERTY(bool signInState
                 READ isSignedIn
                 WRITE setSignedIn
                 NOTIFY signInStateChanged)
  Q_PROPERTY(bool regionChina READ getRegion WRITE setRegion NOTIFY regionChanged)
  Q_PROPERTY(bool darkTheme
                 READ isDarkTheme
                 WRITE setDarkTheme
                 NOTIFY themeChanged)

 public:
  explicit ToolBarMenu(QWidget* parent = nullptr);
  ~ToolBarMenu() override;

  bool isSignedIn() const;

  /**
   * Get current store region.
   * @return true if China is selected, else false.
   */
  bool getRegion() const;

  bool isDarkTheme() const;

 signals:
  void signInStateChanged(bool is_signed_in);
  void themeChanged(bool is_dark_theme);
  void recommendAppRequested();
  void regionChanged(bool is_china);
  void clearCacheRequested();

 public slots:
  void setSignedIn(bool is_signed_in);
  void setRegion(bool is_china);
  void setDarkTheme(bool is_dark_theme);

 private:
  void initActions();

  bool is_signed_in_ = false;
  QAction* sign_in_action_ = nullptr;
  QActionGroup* region_group_ = nullptr;
  QAction* region_china_ = nullptr;
  QAction* region_international_ = nullptr;
  bool is_dark_theme_ = false;
  QAction* switch_theme_action_ = nullptr;

 private slots:
  void onSigninActionTriggered();
  void onThemeActionTriggered();
  void onRegionGroupTriggered(QAction* action);
};

}  // namespace dman

#endif // DEEPIN_APPSTORE_UI_WIDGETS_TOOL_BAR_MENU_H

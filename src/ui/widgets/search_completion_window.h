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

#ifndef DEEPIN_APPSTORE_UI_WIDGETS_SEARCH_COMPLETION_WINDOW_H
#define DEEPIN_APPSTORE_UI_WIDGETS_SEARCH_COMPLETION_WINDOW_H

#include <QFrame>
#include <QListView>
#include <QPushButton>
#include <QStringListModel>

#include "services/search_result.h"

namespace dstore {

class SearchButton;

class SearchCompletionWindow : public QFrame {
  Q_OBJECT
 public:
  explicit SearchCompletionWindow(QWidget* parent = nullptr);
  ~SearchCompletionWindow() override;

  void autoResize();
  const AppSearchRecordList& searchResult() const { return result_; }

 signals:
  void resultClicked(const AppSearchRecord& result);
  void searchButtonClicked();

 public slots:
  void goDown();
  void goUp();
  void onEnterPressed();

  void setKeyword(const QString& keyword);
  void setSearchResult(const AppSearchRecordList& result);

 private:
  void initConnections();
  void initUI();

  QListView* result_view_ = nullptr;
  QStringListModel* model_ = nullptr;
  SearchButton* search_button_ = nullptr;
  AppSearchRecordList result_;

 private slots:
  void onSearchButtonEntered();
  void onResultListClicked(const QModelIndex& index);
  void onResultListEntered(const QModelIndex& index);
};

}  // namespace dstore

#endif  // DEEPIN_APPSTORE_UI_WIDGETS_SEARCH_COMPLETION_WINDOW_H

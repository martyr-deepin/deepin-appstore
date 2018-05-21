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

#include "ui/widgets/search_completion_window.h"

#include <QDebug>
#include <QVBoxLayout>

#include "ui/utils/theme_manager.h"
#include "ui/widgets/search_button.h"

namespace dstore {

namespace {

const int kItemHeight = 25;

}  // namespace

SearchCompletionWindow::SearchCompletionWindow(QWidget* parent)
    : QFrame(parent),
      keyword_() {
  this->setObjectName("SearchCompletionWindow");

  this->initUI();
  this->initConnections();
}

SearchCompletionWindow::~SearchCompletionWindow() {
}

void SearchCompletionWindow::autoResize() {
  result_view_->setFixedHeight(model_->rowCount() * kItemHeight + 2);
  result_view_->setFixedWidth(this->width() - 2);
  search_button_->setFixedWidth(this->width() - 2);
  this->setFixedHeight(result_view_->height() + kItemHeight + 8 + 3);
  result_view_->setVisible(model_->rowCount() > 0);
  this->adjustSize();
  result_view_->raise();
}

void SearchCompletionWindow::goDown() {
  if (model_->rowCount() == 0) {
    search_button_->setChecked(true);
  } else {
    if (search_button_->isChecked()) {
      search_button_->setChecked(false);
      const QModelIndex first_idx = model_->index(0, 0);
      result_view_->setCurrentIndex(first_idx);
    } else {
      const int down_row = result_view_->currentIndex().row() + 1;
      if (down_row >= model_->rowCount()) {
        search_button_->setChecked(true);
        result_view_->setCurrentIndex(QModelIndex());
      } else {
        const QModelIndex down_idx = model_->index(down_row, 0);
        result_view_->setCurrentIndex(down_idx);
      }
    }
  }
}

void SearchCompletionWindow::goUp() {
  if (model_->rowCount() == 0) {
    search_button_->setChecked(true);
  } else {
    if (search_button_->isChecked()) {
      search_button_->setChecked(false);
      // Select last item.
      const QModelIndex idx = model_->index(model_->rowCount() - 1, 0);
      result_view_->setCurrentIndex(idx);
    } else {
      const int up_row = result_view_->currentIndex().row() - 1;
      if (up_row < 0) {
        result_view_->setCurrentIndex(QModelIndex());
        search_button_->setChecked(true);
      } else {
        const QModelIndex up_idx = model_->index(up_row, 0);
        result_view_->setCurrentIndex(up_idx);
      }
    }
  }
}

void SearchCompletionWindow::onEnterPressed() {
  if (search_button_->isChecked()) {
    emit this->searchButtonClicked();
  } else {
    const QModelIndex idx = result_view_->currentIndex();
    this->onResultListClicked(idx);
  }
  // Hide completion window whenever any items was activated.
  this->hide();
}

void SearchCompletionWindow::setKeyword(const QString& keyword) {
  keyword_ = keyword;
  QFontMetrics metrics = search_button_->fontMetrics();
  search_button_->setText(
      metrics.elidedText(
          QObject::tr("Search \"%1\" in Deepin Store").arg(keyword),
          Qt::ElideRight,
          search_button_->width() - 14));
  model_->setStringList(QStringList());
}

void SearchCompletionWindow::setSearchResult(
    const AppSearchRecordList& result) {
  result_ = result;
  QStringList names;
  for (const AppSearchRecord& entry : result) {
    names.append(entry.local_name);
  }
  model_->setStringList(names);
  this->autoResize();
}

void SearchCompletionWindow::initConnections() {
  connect(result_view_, &QListView::activated,
          this, &SearchCompletionWindow::onResultListClicked);
  connect(result_view_, &QListView::entered,
          this, &SearchCompletionWindow::onResultListEntered);
  connect(result_view_, &QListView::pressed,
          this, &SearchCompletionWindow::onResultListClicked);
  connect(search_button_, &SearchButton::entered,
          this, &SearchCompletionWindow::onSearchButtonEntered);
  connect(search_button_, &QPushButton::pressed,
          this, &SearchCompletionWindow::searchButtonClicked);
}

void SearchCompletionWindow::initUI() {
  model_ = new QStringListModel(this);

  result_view_ = new QListView();
  result_view_->setObjectName("ResultList");
  result_view_->setModel(model_);
  result_view_->setMouseTracking(true);
  result_view_->setEditTriggers(QListView::NoEditTriggers);
  result_view_->setSelectionMode(QListView::SingleSelection);
  result_view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  result_view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  result_view_->setFocusPolicy(Qt::NoFocus);
  result_view_->setMinimumHeight(kItemHeight);
  result_view_->adjustSize();

  search_button_ = new SearchButton();
  search_button_->setObjectName("SearchButton");
  search_button_->setCheckable(true);
  search_button_->setFixedHeight(25);
  search_button_->setText(QObject::tr("Search \"%1\" in the full text"));

  QVBoxLayout* main_layout = new QVBoxLayout();
  main_layout->setContentsMargins(0, 4, 0, 3);
  main_layout->setSpacing(0);
  main_layout->addWidget(result_view_, 0, Qt::AlignHCenter | Qt::AlignTop);
  main_layout->addSpacing(1);
  main_layout->addWidget(search_button_, 0, Qt::AlignCenter);
  main_layout->addSpacing(1);

  this->setLayout(main_layout);
  this->setContentsMargins(0, 0, 0, 0);
  this->setMinimumHeight(kItemHeight);
  this->setFixedWidth(242);
  this->setWindowFlags(Qt::FramelessWindowHint |
                       Qt::CustomizeWindowHint |
                       Qt::BypassWindowManagerHint);
  this->setAttribute(Qt::WA_NativeWindow, true);

  ThemeManager::instance()->registerWidget(this);
}

void SearchCompletionWindow::onResultListClicked(const QModelIndex& index) {
  if (index.isValid()) {
    const int row = index.row();
    emit this->resultClicked(result_.at(row));
  } else {
    // Simulate button click event.
    emit this->searchButtonClicked();
  }
}

void SearchCompletionWindow::onSearchButtonEntered() {
  search_button_->setChecked(true);
  result_view_->setCurrentIndex(QModelIndex());
}

void SearchCompletionWindow::onResultListEntered(const QModelIndex& index) {
  result_view_->setCurrentIndex(index);
  search_button_->setChecked(false);
}

}  // namespace dstore
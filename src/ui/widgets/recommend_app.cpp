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

#include "ui/widgets/recommend_app.h"

#include <QGridLayout>
#include <QLabel>

#include "ui/utils/theme_manager.h"

namespace dstore {

RecommendApp::RecommendApp(QWidget* parent) : QDialog(parent) {
  this->setObjectName("RecommendApp");
  this->initUI();
  this->initConnections();
}

RecommendApp::~RecommendApp() {
}

void RecommendApp::clearForm() {
  app_name_->clear();
  app_type_->setCurrentIndex(0);
  home_page_->clear();
  download_addr_->clear();
  misc_->clear();
}

void RecommendApp::initUI() {
  app_name_ = new Dtk::Widget::DLineEdit();
  app_type_ = new QComboBox();
  app_type_->addItems({
                          tr("Linux App"),
                          tr("Windows App"),
                          tr("Web App"),
                          tr("Android App"),
                      });
  home_page_ = new Dtk::Widget::DLineEdit();
  download_addr_ = new Dtk::Widget::DLineEdit();
  misc_ = new Dtk::Widget::DLineEdit();

  QGridLayout* grid_layout = new QGridLayout();
  grid_layout->addWidget(new QLabel(tr("Name")), 0, 0);
  grid_layout->addWidget(app_name_, 0, 1);
  grid_layout->addWidget(new QLabel(tr("Type")), 1, 0);
  grid_layout->addWidget(app_type_, 1, 1);
  grid_layout->addWidget(new QLabel(tr("Homepage")), 2, 0);
  grid_layout->addWidget(home_page_, 2, 1);
  grid_layout->addWidget(new QLabel(tr("Download Add.")), 3, 0);
  grid_layout->addWidget(download_addr_, 3, 1);
  grid_layout->addWidget(new QLabel(tr("Others")), 4, 0);
  grid_layout->addWidget(misc_, 4, 1);

  submit_ = new QPushButton(tr("Submit"));

  QVBoxLayout* layout = new QVBoxLayout();
  layout->addLayout(grid_layout);
  layout->addWidget(submit_);
  this->setLayout(layout);

  this->setWindowTitle(QObject::tr("Recommend App"));
  this->setModal(true);
  ThemeManager::instance()->registerWidget(this);
}

void RecommendApp::initConnections() {
  connect(submit_, &QPushButton::clicked,
          this, &RecommendApp::submit);
}

}  // namespace dstore
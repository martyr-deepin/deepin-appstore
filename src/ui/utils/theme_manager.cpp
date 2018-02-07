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

#include "ui/utils/theme_manager.h"

#include <DThemeManager>
#include <QDebug>
#include <QStyle>
#include <QWidget>

#include "base/file_util.h"

namespace dstore {

namespace {

ThemeManager* g_theme_manager = nullptr;

QString GetQssContent(const QString& theme, const QString& qss_filename) {
  const QString filepath = QString(":/%1/%2.css").arg(theme).arg(qss_filename);
  return ReadFile(filepath);
}

}  // namespace

ThemeManager::ThemeManager(QObject* parent)
    : QObject(parent),
      widgets_(),
      theme_( Dtk::Widget::DThemeManager::instance()->theme()) {
}

ThemeManager::~ThemeManager() {

}

ThemeManager* ThemeManager::instance() {
  if (g_theme_manager == nullptr) {
    g_theme_manager = new ThemeManager();
  }
  Q_ASSERT(g_theme_manager != nullptr);
  return g_theme_manager;
}

void ThemeManager::registerWidget(QWidget* widget) {
  Q_ASSERT(widget != nullptr);
  Q_ASSERT(!widgets_.contains(widget));

  widgets_.append(widget);
  QString qss_filename = widget->property("_d_QSSFilename").toString();
  if (qss_filename.isEmpty()) {
    qss_filename = widget->objectName();
  }
  if (qss_filename.isEmpty()) {
    qss_filename = widget->metaObject()->className();
  }

  widget->style()->unpolish(widget);
  widget->style()->polish(widget);

  // TODO(Shaohua): Save base stylesheet.
  widget->setStyleSheet(GetQssContent(theme_, qss_filename));
  connect(this, &ThemeManager::themeUpdated, [=](const QString& theme) {
    Q_ASSERT(widget != nullptr);
    if (widget != nullptr) {
      widget->setStyleSheet(GetQssContent(theme, qss_filename));
      widget->style()->unpolish(widget);
      widget->style()->polish(widget);
    }
  });
}

void ThemeManager::setTheme(const QString& theme) {
  if (theme == theme_) {
    return;
  }

  theme_ = theme;
  Dtk::Widget::DThemeManager::instance()->setTheme(theme_);
  emit this->themeUpdated(theme_);
}

}  // namespace dstore
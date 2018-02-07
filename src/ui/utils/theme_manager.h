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

#ifndef DEEPIN_APPSTORE_UI_UTILS_THEME_MANAGER_H
#define DEEPIN_APPSTORE_UI_UTILS_THEME_MANAGER_H

#include <QObject>
#include <QVector>

namespace dstore {

// Auto-update styles of registered widgets when theme name is reset.
class ThemeManager : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString theme READ theme WRITE setTheme)

 public:
  // Get global instance of theme manager.
  // NOTE: Only call this method in UI thread.
  static ThemeManager* instance();

  const QString& theme() const {
    return theme_;
  }

  // Let theme manager manages stylesheet of |widget|.
  void registerWidget(QWidget* widget);

 signals:
  // Emitted when setTheme() is called and current theme updated.
  void themeUpdated(const QString& theme);

 public slots:
  void setTheme(const QString& theme);

 private:
  explicit ThemeManager(QObject* parent = nullptr);
  ~ThemeManager() override;

  QVector<QWidget*> widgets_;

  // Current theme name.
  QString theme_;
};

}  // namespace dstore

#endif  // DEEPIN_APPSTORE_UI_UTILS_THEME_MANAGER_H

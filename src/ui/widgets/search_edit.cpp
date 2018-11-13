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

#include "ui/widgets/search_edit.h"

#include <QKeyEvent>

namespace dstore {

SearchEdit::SearchEdit(QWidget* parent) : DSearchEdit(parent) {

}

SearchEdit::~SearchEdit() {

}

void SearchEdit::keyPressEvent(QKeyEvent* event) {
  switch (event->key()) {
    case Qt::Key_Up: {
      emit this->upKeyPressed();
      break;
    }
    case Qt::Key_Down: {
      emit this->downKeyPressed();
      break;
    }
    case Qt::Key_Enter:  // Fall through
    case Qt::Key_Return: {
      emit this->enterPressed();
      break;
    }
    default: {
    }
  }

  DSearchEdit::keyPressEvent(event);
}

}  // namespace dstore

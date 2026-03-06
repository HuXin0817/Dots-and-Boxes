/*
Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2025 Xin Hu <huxin0817.hx@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "BaseCanvas.h"

#include <Dab/Board.h>
#include <Dab/Model.h>
#include <Dab/Tools.h>

#include <QApplication>
#include <QPainter>
#include <QStyleHints>
#include <QWidget>
#include <cstddef>

namespace dab::__detail__::frontend {

QColor BaseCanvas::ThemeColor(const QColor& DarkThemeColor, const QColor& LightThemeColor) {
  switch (QApplication::styleHints()->colorScheme()) {
    case Qt::ColorScheme::Light:
      return LightThemeColor;
    case Qt::ColorScheme::Dark:
      return DarkThemeColor;
    case Qt::ColorScheme::Unknown:
      break;
  }
  return {};
}

const GameBoard& BaseCanvas::GetBoard() const {
  QObject* mainWindow = parent();
  Q_ASSERT(mainWindow != nullptr);
  QVariant value = mainWindow->property("Board");
  Q_ASSERT(value.isValid());
  bool ok;
  qulonglong num = value.toULongLong(&ok);
  Q_ASSERT(ok);
  GameBoard* board = reinterpret_cast<GameBoard*>(num);
  Q_ASSERT(board != nullptr);
  return *board;
}

void BaseCanvas::enterEvent(QEnterEvent* event) {
  QWidget::enterEvent(event);
  HoverState = true;
  update();
}

void BaseCanvas::leaveEvent(QEvent* event) {
  QWidget::leaveEvent(event);
  HoverState = false;
  update();
}

}  // namespace dab::__detail__::frontend
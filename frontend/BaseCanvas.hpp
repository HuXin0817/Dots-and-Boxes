/*
Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2025 Xin Hu <202219120810@stu.cdut.edu.cn>

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

#pragma once

#include <Dab/Model.hpp>
#include <QApplication>
#include <QPainter>
#include <QStyleHints>
#include <QWidget>

namespace dab::detail::frontend {

enum class Owner {
  None,
  Player1,
  Player2,
};

template <int64_t BoardSize>
class BaseCanvas : public QWidget {
 public:
  static constexpr int UnitSize = 6 + 16 / BoardSize;

  using QWidget::QWidget;

  static QColor ThemeColor(const QColor& DarkThemeColor, const QColor& LightThemeColor);
  bool Hovered() const { return HoverState; }
  Owner GetOwner() const { return Owner; }
  void SetOwner(const Turn<BoardSize> turn);

 protected:
  void enterEvent(QEnterEvent* event) override;
  void leaveEvent(QEvent* event) override;

 private:
  Owner Owner = Owner::None;
  bool HoverState = false;
};

template <int64_t BoardSize>
QColor BaseCanvas<BoardSize>::ThemeColor(const QColor& DarkThemeColor, const QColor& LightThemeColor) {
  if (QApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark) {
    return DarkThemeColor;
  } else {
    return LightThemeColor;
  }
}

template <int64_t BoardSize>
void BaseCanvas<BoardSize>::SetOwner(const Turn<BoardSize> turn) {
  if (turn.IsPlayer1Turn()) {
    Owner = Owner::Player1;
  } else {
    Owner = Owner::Player2;
  }
}

template <int64_t BoardSize>
void BaseCanvas<BoardSize>::enterEvent(QEnterEvent* event) {
  QWidget::enterEvent(event);
  HoverState = true;
  update();
}

template <int64_t BoardSize>
void BaseCanvas<BoardSize>::leaveEvent(QEvent* event) {
  QWidget::leaveEvent(event);
  HoverState = false;
  update();
}

}  // namespace dab::detail::frontend
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

#include <Dab/Common.h>

#include <QApplication>
#include <QWidget>

namespace dab::__detail__::frontend {

enum class Owner {
  None,
  Player1,
  Player2,
};

class BaseCanvas : public QWidget {
  Q_OBJECT

 public:
  static constexpr int UnitSize = 6 + 16 / BoardSize;

  using QWidget::QWidget;

  static QColor ThemeColor(const QColor& DarkThemeColor, const QColor& LightThemeColor);
  bool Hovered() const { return HoverState; }
  Owner GetOwner() const { return Owner; }
  void SetOwner(Owner owner) { Owner = owner; }
  void SetOwner(Turn turn);

 protected:
  void enterEvent(QEnterEvent* event) override;
  void leaveEvent(QEvent* event) override;

 private:
  Owner Owner = Owner::None;
  bool HoverState = false;
};

}  // namespace dab::__detail__::frontend
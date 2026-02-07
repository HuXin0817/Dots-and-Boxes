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

#include "EdgeCanvas.hpp"

#include "BaseCanvas.hpp"

namespace dab::detail::frontend {

EdgeCanvas::EdgeCanvas(bool rotate, const std::function<void()>& callBack, QWidget* parent)
    : BaseCanvas(parent), CallBack(callBack) {
  BaseCanvas::setFixedSize(rotate ? Width : Height, rotate ? Height : Width);
}

void EdgeCanvas::mousePressEvent(QMouseEvent* event) {
  QWidget::mousePressEvent(event);

  CallBack();
}

void EdgeCanvas::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::NoPen);
  painter.setBrush(QBrush(Color()));
  painter.drawRect(BaseCanvas::rect());
}

QColor EdgeCanvas::Color() const {
  static constexpr QColor DarkThemeColor = QColor(65, 65, 65, 255);
  static constexpr QColor LightThemeColor = QColor(217, 217, 217, 255);
  static constexpr QColor DarkThemeHoveredColor = QColor(90, 90, 90, 255);
  static constexpr QColor LightThemeHoveredColor = QColor(202, 202, 202, 255);
  static constexpr QColor Player1OccupyColor = QColor(64, 64, 255, 255);
  static constexpr QColor Player2OccupyColor = QColor(255, 64, 64, 255);

  if (BaseCanvas::GetOwner() == Owner::None) {
    if (BaseCanvas::Hovered()) {
      return BaseCanvas::ThemeColor(DarkThemeHoveredColor, LightThemeHoveredColor);
    }
    return BaseCanvas::ThemeColor(DarkThemeColor, LightThemeColor);
  }

  QColor color;
  if (BaseCanvas::GetOwner() == Owner::Player1) {
    color = Player1OccupyColor;
  } else if (BaseCanvas::GetOwner() == Owner::Player2) {
    color = Player2OccupyColor;
  }
  if (HighLight) {
    color.setAlpha(255);
  } else if (BaseCanvas::Hovered()) {
    color.setAlpha(144);
  } else {
    color.setAlpha(128);
  }

  return color;
}

}  // namespace dab::detail::frontend
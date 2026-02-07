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

#include "BaseCanvas.hpp"

namespace dab::detail::frontend {

template <int64_t BoardSize>
class DotCanvas final : public BaseCanvas<BoardSize> {
 public:
  static constexpr int Width = 2 * BaseCanvas<BoardSize>::UnitSize;

  explicit DotCanvas(QWidget* parent);

 protected:
  void
  paintEvent(QPaintEvent* event) override;

 private:
  QColor
  Color() const;
};

template <int64_t BoardSize>
DotCanvas<BoardSize>::DotCanvas(QWidget* parent) : BaseCanvas<BoardSize>(parent) {
  BaseCanvas<BoardSize>::setFixedSize(Width, Width);
}

template <int64_t BoardSize>
void
DotCanvas<BoardSize>::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setBrush(QBrush(Color()));
  painter.setPen(Qt::NoPen);

  const int x = BaseCanvas<BoardSize>::width() / 2;
  const int y = BaseCanvas<BoardSize>::height() / 2;
  painter.drawEllipse(QPoint(x, y), BaseCanvas<BoardSize>::UnitSize, BaseCanvas<BoardSize>::UnitSize);
}

template <int64_t BoardSize>
QColor
DotCanvas<BoardSize>::Color() const {
  static constexpr QColor DarkThemeColor = QColor(202, 202, 202, 255);
  static constexpr QColor LightThemeColor = QColor(255, 255, 255, 255);

  return BaseCanvas<BoardSize>::ThemeColor(DarkThemeColor, LightThemeColor);
}

}  // namespace dab::detail::frontend
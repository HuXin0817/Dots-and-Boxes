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
#include "EdgeCanvas.hpp"

namespace dab::detail::frontend {

template <int64_t BoardSize>
class BoxCanvas final : public BaseCanvas<BoardSize> {
 public:
  static constexpr int Width = EdgeCanvas<BoardSize>::Height - 2 * BaseCanvas<BoardSize>::UnitSize;

  explicit BoxCanvas(QWidget* parent);

 protected:
  void
  paintEvent(QPaintEvent* event) override;

 private:
  QColor
  Color() const;
};

template <int64_t BoardSize>
BoxCanvas<BoardSize>::BoxCanvas(QWidget* parent) : BaseCanvas<BoardSize>(parent) {
  BaseCanvas<BoardSize>::setFixedSize(Width, Width);
}

template <int64_t BoardSize>
void
BoxCanvas<BoardSize>::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::NoPen);
  painter.setBrush(QBrush(Color()));
  painter.drawRect(BaseCanvas<BoardSize>::rect());
}

template <int64_t BoardSize>
QColor
BoxCanvas<BoardSize>::Color() const {
  static constexpr QColor Player1OccupyColor = QColor(64, 64, 255, 64);
  static constexpr QColor Player2OccupyColor = QColor(255, 64, 64, 64);

  if (BaseCanvas<BoardSize>::GetOwner() == Owner::None) {
    return QColor(0, 0, 0, 0);
  } else if (BaseCanvas<BoardSize>::GetOwner() == Owner::Player1) {
    return Player1OccupyColor;
  } else {
    return Player2OccupyColor;
  }
}

}  // namespace dab::detail::frontend
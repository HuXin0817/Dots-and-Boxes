#pragma once

#include <QPaintEvent>
#include <QPainter>

#include "BaseCanvas.hpp"

namespace dab::frontend::canvas {

template <int BoardSize, typename SizeType>
class DotCanvas final : public BaseCanvas<BoardSize, SizeType> {
  public:
  static constexpr int Width = 2 * BaseCanvas<BoardSize, SizeType>::UnitSize;

  explicit DotCanvas(QWidget* parent) : BaseCanvas<BoardSize, SizeType>(parent) {
    BaseCanvas<BoardSize, SizeType>::setFixedSize(Width, Width);
  }

  QColor
  Color() const override {
    static QColor DarkThemeColor = {202, 202, 202, 255};
    static QColor LightThemeColor = {255, 255, 255, 255};

    return BaseCanvas<BoardSize, SizeType>::isDarkTheme() ? DarkThemeColor : LightThemeColor;
  }

  void
  paintEvent(QPaintEvent* event) override {
    BaseCanvas<BoardSize, SizeType>::paintEvent(event);

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(Color()));
    painter.setPen(Qt::NoPen);

    const int x = BaseCanvas<BoardSize, SizeType>::width() / 2;
    const int y = BaseCanvas<BoardSize, SizeType>::height() / 2;

    painter.drawEllipse(
        QPoint(x, y), BaseCanvas<BoardSize, SizeType>::UnitSize, BaseCanvas<BoardSize, SizeType>::UnitSize);
  }
};

}  // namespace dab::frontend::canvas

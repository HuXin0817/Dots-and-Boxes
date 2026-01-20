#pragma once

#include <QPaintEvent>
#include <QPainter>

#include "BaseCanvas.hpp"

namespace dab::frontend::canvas {

template <int BoardSize>
class DotCanvas final : public BaseCanvas<BoardSize> {
  public:
  static constexpr int Width = 2 * BaseCanvas<BoardSize>::UnitSize;

  explicit DotCanvas(QWidget* parent) : BaseCanvas<BoardSize>(parent) {
    BaseCanvas<BoardSize>::setFixedSize(Width, Width);
  }

  QColor
  Color() const override {
    static QColor DarkThemeColor = {202, 202, 202, 255};
    static QColor LightThemeColor = {255, 255, 255, 255};

    return BaseCanvas<BoardSize>::isDarkTheme() ? DarkThemeColor : LightThemeColor;
  }

  void
  paintEvent(QPaintEvent* event) override {
    BaseCanvas<BoardSize>::paintEvent(event);

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(Color()));
    painter.setPen(Qt::NoPen);

    const int x = BaseCanvas<BoardSize>::width() / 2;
    const int y = BaseCanvas<BoardSize>::height() / 2;

    painter.drawEllipse(QPoint(x, y), BaseCanvas<BoardSize>::UnitSize, BaseCanvas<BoardSize>::UnitSize);
  }
};

}  // namespace dab::frontend::canvas

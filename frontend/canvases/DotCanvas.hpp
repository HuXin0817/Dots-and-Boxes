#pragma once

#include <QPaintEvent>
#include <QPainter>

#include "BaseCanvas.hpp"

template <int BoardSize>
class DotCanvas final : public BaseCanvas<BoardSize> {
  public:
  static constexpr int Width = 2 * BaseCanvas<BoardSize>::UnitSize;

  static inline QColor DarkThemeColor = {202, 202, 202, 255};
  static inline QColor LightThemeColor = {255, 255, 255, 255};

  QColor
  Color() const override {
    return BaseCanvas<BoardSize>::isDarkTheme() ? DarkThemeColor : LightThemeColor;
  }

  explicit DotCanvas(QWidget* parent) : BaseCanvas<BoardSize>(parent) {
    BaseCanvas<BoardSize>::setFixedSize(Width, Width);
  }

  void
  paintEvent(QPaintEvent* event) override {
    BaseCanvas<BoardSize>::paintEvent(event);

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(Color()));
    painter.setPen(Qt::NoPen);

    int x = BaseCanvas<BoardSize>::width() / 2;
    int y = BaseCanvas<BoardSize>::height() / 2;

    painter.drawEllipse(
        QPoint(x, y), BaseCanvas<BoardSize>::UnitSize, BaseCanvas<BoardSize>::UnitSize);
  }
};

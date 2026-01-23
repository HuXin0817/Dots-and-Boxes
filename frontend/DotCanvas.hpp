#pragma once

#include "BaseCanvas.hpp"

template <int64_t BoardSize>
class DotCanvas final : public BaseCanvas<BoardSize> {
  using Base = BaseCanvas<BoardSize>;

  public:
  static constexpr int Width = 2 * Base::UnitSize;

  explicit DotCanvas(QWidget* parent) : Base(parent) {
    Base::setFixedSize(Width, Width);
  }

  QColor
  Color() const override {
    static QColor DarkThemeColor = {202, 202, 202, 255};
    static QColor LightThemeColor = {255, 255, 255, 255};

    return Base::isDarkTheme() ? DarkThemeColor : LightThemeColor;
  }

  void
  paintEvent(QPaintEvent* event) override {
    Base::paintEvent(event);

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(Color()));
    painter.setPen(Qt::NoPen);

    const int x = Base::width() / 2;
    const int y = Base::height() / 2;

    painter.drawEllipse(QPoint(x, y), Base::UnitSize, Base::UnitSize);
  }
};

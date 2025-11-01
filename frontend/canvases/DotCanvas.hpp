#pragma once

#include <QPaintEvent>
#include <QPainter>

#include "BaseCanvas.hpp"

class DotCanvas final : public BaseCanvas {
  Q_OBJECT

  public:
  static constexpr int Width = 2 * UnitSize;

  QColor
  Color() const override {
    if (isDarkTheme()) {
      return {202, 202, 202, 255};
    } else {
      return {255, 255, 255, 255};
    }
  }

  explicit DotCanvas(QWidget* parent) : BaseCanvas(parent) {
    setFixedSize(Width, Width);
  }

  void
  paintEvent(QPaintEvent* event) override {
    BaseCanvas::paintEvent(event);

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(Color()));
    painter.setPen(Qt::NoPen);

    int x = width() / 2;
    int y = height() / 2;

    painter.drawEllipse(QPoint(x, y), UnitSize, UnitSize);
  }
};

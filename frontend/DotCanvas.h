#pragma once

#include <QPaintEvent>
#include <QPainter>

#include "BaseCanvas.h"

class DotCanvas final : public BaseCanvas {
  Q_OBJECT

  public:
  static constexpr int A = 2 * R;

  QColor
  Color() const override {
    if (isDarkTheme()) {
      return {202, 202, 202, 255};
    } else {
      return {255, 255, 255, 255};
    }
  }

  explicit DotCanvas(QWidget* parent) : BaseCanvas(parent) {
    setFixedSize(A, A);
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

    painter.drawEllipse(QPoint(x, y), R, R);
  }
};

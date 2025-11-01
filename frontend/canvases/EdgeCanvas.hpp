#pragma once

#include "DotCanvas.hpp"

class EdgeCanvas final : public BaseCanvas {
  Q_OBJECT

  public:
  static constexpr int A = R * 2;
  static constexpr int B = A * 5;

  explicit EdgeCanvas(bool rotate, QWidget* parent) : BaseCanvas(parent), Rotate(rotate) {
    if (Rotate) {
      resize(QSize(A, B));
    } else {
      resize(QSize(B, A));
    }
  }

  CanvasState State = CanvasState::Free;
  bool HighLight = true;
  bool Rotate = false;

  QColor
  Color() const override {
    if (State == CanvasState::Free) {
      if (isDarkTheme()) {
        return {65, 65, 65, 255};
      } else {
        return {217, 217, 217, 255};
      }
    }

    QColor color;
    if (State == CanvasState::Player1Occupy) {
      color = {64, 64, 255, 255};
    }

    if (State == CanvasState::Player2Occupy) {
      color = {255, 64, 64, 255};
    }

    if (HighLight) {
      color.setAlpha(255);
    } else {
      color.setAlpha(128);
    }

    return color;
  }

  protected:
  void
  paintEvent(QPaintEvent* event) override {
    BaseCanvas::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Color()));

    if (Rotate) {
      int x = width() / 2 - A / 2;
      int y = height() / 2 - B / 2;
      painter.drawRect(x, y, A, B);
    } else {
      int x = width() / 2 - B / 2;
      int y = height() / 2 - A / 2;
      painter.drawRect(x, y, B, A);
    }
  }
};

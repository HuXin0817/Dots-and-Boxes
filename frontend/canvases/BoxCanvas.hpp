#pragma once

#include "BaseCanvas.hpp"
#include "EdgeCanvas.hpp"

class BoxCanvas final : public BaseCanvas {
  public:
  static constexpr int Width = EdgeCanvas::Height - 2 * UnitSize;

  explicit BoxCanvas(QWidget* parent) : BaseCanvas(parent) {
    resize(QSize(Width, Width));
  }

  static inline QColor Player1OccupyColor = {64, 64, 255, 64};
  static inline QColor Player2OccupyColor = {255, 64, 64, 64};

  QColor
  Color() const override {
    if (State == CanvasState::Player1Occupy) {
      return Player1OccupyColor;
    }
    if (State == CanvasState::Player2Occupy) {
      return Player2OccupyColor;
    }
    return {0, 0, 0, 0};
  }

  void
  SetState(CanvasState State) {
    this->State = State;
  }

  protected:
  void
  paintEvent(QPaintEvent* event) override {
    BaseCanvas::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Color()));

    int x = width() / 2 - Width / 2;
    int y = height() / 2 - Width / 2;

    painter.drawRect(x, y, Width, Width);
  }

  private:
  CanvasState State = CanvasState::Free;
};

#pragma once

#include "BaseCanvas.hpp"
#include "EdgeCanvas.hpp"

template <int64_t BoardSize>
class BoxCanvas final : public BaseCanvas<BoardSize> {
  using Base = BaseCanvas<BoardSize>;

  public:
  static constexpr int Width = EdgeCanvas<BoardSize>::Height - 2 * Base::UnitSize;

  explicit BoxCanvas(QWidget* parent) : Base(parent) {
    Base::resize(QSize(Width, Width));
  }

  QColor
  Color() const {
    static QColor Player1OccupyColor = {64, 64, 255, 64};
    static QColor Player2OccupyColor = {255, 64, 64, 64};

    if (Base::State == Base::State::Player1Occupy) {
      return Player1OccupyColor;
    }
    if (Base::State == Base::State::Player2Occupy) {
      return Player2OccupyColor;
    }
    return {0, 0, 0, 0};
  }

  protected:
  void
  paintEvent(QPaintEvent* event) override {
    Base::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Color()));

    painter.drawRect(Base::rect());
  }
};

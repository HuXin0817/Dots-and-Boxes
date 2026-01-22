#pragma once

#include "BaseCanvas.hpp"
#include "EdgeCanvas.hpp"

namespace dab::frontend::canvas {

template <int BoardSize, typename SizeType>
class BoxCanvas final : public BaseCanvas<BoardSize, SizeType> {
  public:
  static constexpr int Width = EdgeCanvas<BoardSize, SizeType>::Height - 2 * BaseCanvas<BoardSize, SizeType>::UnitSize;

  explicit BoxCanvas(QWidget* parent) : BaseCanvas<BoardSize, SizeType>(parent) {
    BaseCanvas<BoardSize, SizeType>::resize(QSize(Width, Width));
  }

  QColor
  Color() const override {
    static QColor Player1OccupyColor = {64, 64, 255, 64};
    static QColor Player2OccupyColor = {255, 64, 64, 64};

    if (BaseCanvas<BoardSize, SizeType>::State == BaseCanvas<BoardSize, SizeType>::CanvasState::Player1Occupy) {
      return Player1OccupyColor;
    }
    if (BaseCanvas<BoardSize, SizeType>::State == BaseCanvas<BoardSize, SizeType>::CanvasState::Player2Occupy) {
      return Player2OccupyColor;
    }
    return {0, 0, 0, 0};
  }

  protected:
  void
  paintEvent(QPaintEvent* event) override {
    BaseCanvas<BoardSize, SizeType>::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Color()));

    const int x = BaseCanvas<BoardSize, SizeType>::width() / 2 - Width / 2;
    const int y = BaseCanvas<BoardSize, SizeType>::height() / 2 - Width / 2;

    painter.drawRect(x, y, Width, Width);
  }
};

}  // namespace dab::frontend::canvas

#pragma once

#include <QPainter>

#include "BaseCanvas.hpp"
#include "BaseEdgeCanvas.hpp"

namespace dab::frontend::canvas {

template <int BoardSize, typename SizeType>
class EdgeCanvas final : public BaseEdgeCanvas<BoardSize, SizeType> {
  public:
  explicit EdgeCanvas(const bool rotate, QWidget* parent) : BaseEdgeCanvas<BoardSize, SizeType>(rotate, parent) {
  }

  QColor
  Color() const override {
    static QColor DarkThemeColor = {65, 65, 65, 255};
    static QColor LightThemeColor = {217, 217, 217, 255};
    static QColor Player1OccupyColor = {64, 64, 255, 255};
    static QColor Player2OccupyColor = {255, 64, 64, 255};

    if (BaseCanvas<BoardSize, SizeType>::State == BaseEdgeCanvas<BoardSize, SizeType>::State::Free) {
      return BaseEdgeCanvas<BoardSize, SizeType>::isDarkTheme() ? DarkThemeColor : LightThemeColor;
    }

    QColor color;
    if (BaseCanvas<BoardSize, SizeType>::State == BaseEdgeCanvas<BoardSize, SizeType>::State::Player1Occupy) {
      color = Player1OccupyColor;
    } else if (BaseCanvas<BoardSize, SizeType>::State == BaseEdgeCanvas<BoardSize, SizeType>::State::Player2Occupy) {
      color = Player2OccupyColor;
    }

    if (HighLight) {
      color.setAlpha(255);
    } else {
      color.setAlpha(128);
    }

    return color;
  }

  void
  SetHighLight(const bool highLight) {
    HighLight = highLight;
  }

  protected:
  void
  paintEvent(QPaintEvent* event) override {
    BaseEdgeCanvas<BoardSize, SizeType>::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Color()));

    painter.drawRect(
        BaseEdgeCanvas<BoardSize, SizeType>::width() / 2 - BaseEdgeCanvas<BoardSize, SizeType>::RotateWidth() / 2,
        BaseEdgeCanvas<BoardSize, SizeType>::height() / 2 - BaseEdgeCanvas<BoardSize, SizeType>::RotateHeight() / 2,
        BaseEdgeCanvas<BoardSize, SizeType>::RotateWidth(),
        BaseEdgeCanvas<BoardSize, SizeType>::RotateHeight());
  }

  private:
  bool HighLight = true;
};

}  // namespace dab::frontend::canvas

#pragma once

#include <QPainter>

#include "BaseEdgeCanvas.hpp"

template <int BoardSize>
class EdgeCanvas final : public BaseEdgeCanvas<BoardSize> {
  public:
  explicit EdgeCanvas(bool rotate, QWidget* parent) : BaseEdgeCanvas<BoardSize>(rotate, parent) {
  }

  QColor
  Color() const override {
    static QColor DarkThemeColor = {65, 65, 65, 255};
    static QColor LightThemeColor = {217, 217, 217, 255};
    static QColor Player1OccupyColor = {64, 64, 255, 255};
    static QColor Player2OccupyColor = {255, 64, 64, 255};

    if (State == BaseEdgeCanvas<BoardSize>::CanvasState::Free) {
      return BaseEdgeCanvas<BoardSize>::isDarkTheme() ? DarkThemeColor : LightThemeColor;
    }

    QColor color;
    if (State == BaseEdgeCanvas<BoardSize>::CanvasState::Player1Occupy) {
      color = Player1OccupyColor;
    } else if (State == BaseEdgeCanvas<BoardSize>::CanvasState::Player2Occupy) {
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
  SetHighLight(bool highLight) {
    HighLight = highLight;
  }

  void
  SetState(typename BaseEdgeCanvas<BoardSize>::CanvasState state) {
    State = state;
  }

  protected:
  void
  paintEvent(QPaintEvent* event) override {
    BaseEdgeCanvas<BoardSize>::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Color()));

    painter.drawRect(
        BaseEdgeCanvas<BoardSize>::width() / 2 - BaseEdgeCanvas<BoardSize>::RotateWidth() / 2,
        BaseEdgeCanvas<BoardSize>::height() / 2 - BaseEdgeCanvas<BoardSize>::RotateHeight() / 2,
        BaseEdgeCanvas<BoardSize>::RotateWidth(),
        BaseEdgeCanvas<BoardSize>::RotateHeight());
  }

  private:
  typename BaseEdgeCanvas<BoardSize>::CanvasState State =
      BaseEdgeCanvas<BoardSize>::CanvasState::Free;
  bool HighLight = true;
};

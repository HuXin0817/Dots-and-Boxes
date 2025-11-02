#pragma once

#include "BaseEdgeCanvas.hpp"
#include "DotCanvas.hpp"

class EdgeCanvas final : public BaseEdgeCanvas {
  Q_OBJECT

  public:
  explicit EdgeCanvas(bool rotate, QWidget* parent) : BaseEdgeCanvas(rotate, parent) {
  }

  CanvasState State = CanvasState::Free;
  bool HighLight = true;

  static inline QColor DarkThemeColor = {65, 65, 65, 255};
  static inline QColor LightThemeColor = {217, 217, 217, 255};
  static inline QColor Player1OccupyColor = {64, 64, 255, 255};
  static inline QColor Player2OccupyColor = {255, 64, 64, 255};

  QColor
  Color() const override {
    if (State == CanvasState::Free) {
      return isDarkTheme() ? DarkThemeColor : LightThemeColor;
    }

    QColor color;
    if (State == CanvasState::Player1Occupy) {
      color = Player1OccupyColor;
    } else if (State == CanvasState::Player2Occupy) {
      color = Player2OccupyColor;
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
    BaseEdgeCanvas::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Color()));

    int x = width() / 2 - RotateWidth() / 2;
    int y = height() / 2 - RotateHeight() / 2;
    painter.drawRect(x, y, RotateWidth(), RotateHeight());
  }
};

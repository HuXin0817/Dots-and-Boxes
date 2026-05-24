#include "EdgeCanvas.h"

#include <QPainter>

#include "../src/Board.h"
#include "BaseCanvas.h"

namespace dab::__detail__::frontend {

void EdgeCanvas::mousePressEvent(QMouseEvent* event) {
  QWidget::mousePressEvent(event);

  Callback(Value);
}

void EdgeCanvas::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::NoPen);
  painter.setBrush(QBrush(Color()));
  painter.drawRect(rect());
}

QColor EdgeCanvas::Color() const {
  static constexpr QColor DarkThemeColor = QColor(65, 65, 65, 255);
  static constexpr QColor LightThemeColor = QColor(217, 217, 217, 255);
  static constexpr QColor DarkThemeHoveredColor = QColor(90, 90, 90, 255);
  static constexpr QColor LightThemeHoveredColor = QColor(202, 202, 202, 255);
  static constexpr QColor Player1OccupyColor = QColor(64, 64, 255, 255);
  static constexpr QColor Player2OccupyColor = QColor(255, 64, 64, 255);

  const GameBoard& board = GetBoard();
  if (board.GetOwner(Value) == Owner::None) {
    if (Hovered()) {
      return ThemeColor(DarkThemeHoveredColor, LightThemeHoveredColor);
    }

    return ThemeColor(DarkThemeColor, LightThemeColor);
  }

  QColor color;
  if (board.GetOwner(Value) == Owner::Player1) {
    color = Player1OccupyColor;
  } else if (board.GetOwner(Value) == Owner::Player2) {
    color = Player2OccupyColor;
  }

  if (board.MoveRecord().Back() == Value) {
    color.setAlpha(255);
  } else if (Hovered()) {
    color.setAlpha(144);
  } else {
    color.setAlpha(128);
  }

  return color;
}

}  // namespace dab::__detail__::frontend
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
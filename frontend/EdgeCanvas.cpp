#include "EdgeCanvas.h"

#include <QPainter>

#include "../src/Board.h"
#include "GlobalEnv.h"

namespace dab::__detail__::frontend {

void EdgeCanvas::mousePressEvent(QMouseEvent* event) {
  QWidget::mousePressEvent(event);
  Env->SetHumanMoveEdge(Value);
}

void EdgeCanvas::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::RenderHint::Antialiasing);
  painter.setPen(Qt::PenStyle::NoPen);
  painter.setBrush(QBrush(Color()));
  painter.drawRect(rect());
}

void EdgeCanvas::enterEvent(QEnterEvent* event) {
  QWidget::enterEvent(event);
  Hovered = true;
  update();
}

void EdgeCanvas::leaveEvent(QEvent* event) {
  QWidget::leaveEvent(event);
  Hovered = false;
  update();
}

QColor EdgeCanvas::Color() const {
  if (Env->GetBoard().GetOwner(Value) == Owner::None) {
    if (Hovered) {
      return Env->ThemeColor(DarkThemeHoveredColor, LightThemeHoveredColor);
    }

    return Env->ThemeColor(DarkThemeColor, LightThemeColor);
  }

  QColor color;
  if (Env->GetBoard().GetOwner(Value) == Owner::Player1) {
    color = Player1OccupyColor;
  } else if (Env->GetBoard().GetOwner(Value) == Owner::Player2) {
    color = Player2OccupyColor;
  }

  if (Env->GetBoard().MoveRecord().Back() == Value) {
    color.setAlpha(255);
  } else if (Hovered) {
    color.setAlpha(144);
  } else {
    color.setAlpha(128);
  }

  return color;
}

}  // namespace dab::__detail__::frontend
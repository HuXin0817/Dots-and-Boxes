#include "EdgeCanvas.h"

#include <QPainter>

#include "../Board.h"

namespace dab::__detail__::frontend {

QSize EdgeCanvas::Size() const {
  int x = Value.Rotate() ? Env->DotDiameter() : Env->EdgeWidth();
  int y = Value.Rotate() ? Env->EdgeWidth() : Env->DotDiameter();
  return {x, y};
}

QPoint EdgeCanvas::Pos() const {
  int x = Env->Padding() + Value.Dot1().X() * Env->EdgeWidth();
  int y = Env->Padding() + Value.Dot1().Y() * Env->EdgeWidth();
  if (Value.Rotate()) {
    y += Env->DotRadius();
  } else {
    x += Env->DotRadius();
  }
  return {x, y};
}

QColor EdgeCanvas::Color() const {
  if (Env->GetBoard().GetOwner(Value) == board::Owner::None) {
    if (Hovered) {
      return Env->ThemeColor(DarkThemeHoveredColor, LightThemeHoveredColor);
    }

    return Env->ThemeColor(DarkThemeColor, LightThemeColor);
  }

  QColor color;
  if (Env->GetBoard().GetOwner(Value) == board::Owner::Player1) {
    color = Player1OccupyColor;
  } else if (Env->GetBoard().GetOwner(Value) == board::Owner::Player2) {
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

}  // namespace dab::__detail__::frontend
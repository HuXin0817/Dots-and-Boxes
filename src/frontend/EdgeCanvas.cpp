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

void EdgeCanvas::mousePressEvent(QMouseEvent* event) {
  QWidget::mousePressEvent(event);
  Env->SetHumanMoveEdge(Value);
}

void EdgeCanvas::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  bool highlight = Env->GetBoard().MoveRecord().Back() == Value;
  board::Owner owner = Env->GetBoard().GetOwner(Value);

  QPainter painter(this);
  painter.setRenderHint(QPainter::RenderHint::Antialiasing);
  painter.setPen(Qt::PenStyle::NoPen);
  painter.setBrush(QBrush(Env->EdgeColor(Hovered, highlight, owner)));
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
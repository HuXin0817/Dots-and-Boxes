#include "BoxCanvas.h"

#include <QPainter>

namespace dab::__detail__::frontend {

QPoint BoxCanvas::Pos() const {
  int x = Value.X() * Env->EdgeWidth() + Env->BoxPadding();
  int y = Value.Y() * Env->EdgeWidth() + Env->BoxPadding();
  return {x, y};
}

QColor BoxCanvas::Color() const {
  switch (Env->GetBoard().GetOwner(Value)) {
    case board::Owner::None:
      return {0, 0, 0, 0};
    case board::Owner::Player1:
      return Player1OccupyColor;
    case board::Owner::Player2:
      return Player2OccupyColor;
  }

  return {0, 0, 0, 0};
}

void BoxCanvas::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::RenderHint::Antialiasing);
  painter.setPen(Qt::PenStyle::NoPen);
  painter.setBrush(QBrush(Color()));
  painter.drawRect(rect());
}

}  // namespace dab::__detail__::frontend
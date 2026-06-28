#include "BoxCanvas.h"

#include <QPainter>

namespace dab::__detail__::frontend {

QPoint BoxCanvas::Pos() const {
  int x = Value.X() * Env->EdgeWidth() + Env->BoxPadding();
  int y = Value.Y() * Env->EdgeWidth() + Env->BoxPadding();
  return {x, y};
}

void BoxCanvas::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::RenderHint::Antialiasing);
  painter.setPen(Qt::PenStyle::NoPen);
  painter.setBrush(QBrush(Env->BoxColor(Env->GetBoard().GetOwner(Value))));
  painter.drawRect(rect());
}

}  // namespace dab::__detail__::frontend
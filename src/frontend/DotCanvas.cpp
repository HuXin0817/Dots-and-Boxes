#include "DotCanvas.h"

#include <QPainter>

namespace dab::__detail__::frontend {

QPoint DotCanvas::Pos() const {
  int x = Value.X() * Env->EdgeWidth() + Env->Padding();
  int y = Value.Y() * Env->EdgeWidth() + Env->Padding();
  return {x, y};
}

void DotCanvas::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::RenderHint::Antialiasing);
  painter.setBrush(QBrush(Env->DotColor()));
  painter.setPen(Qt::PenStyle::NoPen);

  const int x = width() / 2;
  const int y = height() / 2;
  painter.drawEllipse(QPoint(x, y), x, y);
}

}  // namespace dab::__detail__::frontend
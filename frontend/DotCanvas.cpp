#include "DotCanvas.h"

#include <QPainter>

namespace dab::__detail__::frontend {

void DotCanvas::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setBrush(QBrush(Color()));
  painter.setPen(Qt::NoPen);

  const int x = width() / 2;
  const int y = height() / 2;
  painter.drawEllipse(QPoint(x, y), UnitSize, UnitSize);
}

}  // namespace dab::__detail__::frontend
#include "BoxCanvas.h"

#include <QPainter>

namespace dab::__detail__::frontend {

void BoxCanvas::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::NoPen);
  painter.setBrush(QBrush(Color()));
  painter.drawRect(rect());
}

QColor BoxCanvas::Color() const {
  switch (Env->GetBoard().GetOwner(Value)) {
    case Owner::None:
      return {0, 0, 0, 0};
    case Owner::Player1:
      return Player1OccupyColor;
    case Owner::Player2:
      return Player2OccupyColor;
  }

  return {0, 0, 0, 0};
}

}  // namespace dab::__detail__::frontend
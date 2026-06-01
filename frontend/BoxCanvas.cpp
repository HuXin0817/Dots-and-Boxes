#include "BoxCanvas.h"

#include <QPainter>

#include "EdgeCanvas.h"
#include "MainWindow.h"

namespace dab::__detail__::frontend {

QSize BoxCanvas::Size() const {
  int width = Width(Env->GetUnitSize());
  return {width, width};
}

QPoint BoxCanvas::Pos() const {
  int offset = MainWindow::StartOffset(Env->GetUnitSize()) + 2 * Env->GetUnitSize();
  int height = EdgeCanvas::Height(Env->GetUnitSize());
  return {offset + Value.X() * height, offset + Value.Y() * height};
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

void BoxCanvas::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::RenderHint::Antialiasing);
  painter.setPen(Qt::PenStyle::NoPen);
  painter.setBrush(QBrush(Color()));
  painter.drawRect(rect());
}

}  // namespace dab::__detail__::frontend
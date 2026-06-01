#include "DotCanvas.h"

#include <QPainter>

#include "EdgeCanvas.h"
#include "MainWindow.h"

namespace dab::__detail__::frontend {

QSize DotCanvas::Size() const {
  int width = Width(Env->GetUnitSize());
  return {width, width};
}

QPoint DotCanvas::Pos() const {
  int offset = MainWindow::StartOffset(Env->GetUnitSize());
  int height = EdgeCanvas::Height(Env->GetUnitSize());
  return {offset + Value.X() * height, offset + Value.Y() * height};
}

void DotCanvas::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::RenderHint::Antialiasing);
  painter.setBrush(QBrush(Color()));
  painter.setPen(Qt::PenStyle::NoPen);

  const int x = width() / 2;
  const int y = height() / 2;
  painter.drawEllipse(QPoint(x, y), x, y);
}

}  // namespace dab::__detail__::frontend
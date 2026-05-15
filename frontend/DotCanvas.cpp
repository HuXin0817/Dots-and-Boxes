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

QColor DotCanvas::Color() const {
  static constexpr QColor DarkThemeColor = QColor(202, 202, 202, 255);
  static constexpr QColor LightThemeColor = QColor(255, 255, 255, 255);

  return ThemeColor(DarkThemeColor, LightThemeColor);
}

}  // namespace dab::__detail__::frontend
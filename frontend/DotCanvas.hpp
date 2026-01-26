#pragma once

#include "Common.hpp"

namespace dab::frontend {

template <int64_t BoardSize>
class DotCanvas final : public QWidget {
  public:
  static constexpr int Width = 2 * UnitSize<BoardSize>;

  explicit DotCanvas(QPointer<QWidget> parent);

  protected:
  void
  paintEvent(QPaintEvent* event) override;

  private:
  QColor
  Color() const;
};

template <int64_t BoardSize>
DotCanvas<BoardSize>::DotCanvas(QPointer<QWidget> parent) : QWidget(parent) {
  setFixedSize(Width, Width);
}

template <int64_t BoardSize>
void
DotCanvas<BoardSize>::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  QPainter painter(this);

  painter.setRenderHint(QPainter::Antialiasing);
  painter.setBrush(QBrush(Color()));
  painter.setPen(Qt::NoPen);

  const int x = width() / 2;
  const int y = height() / 2;

  painter.drawEllipse(QPoint(x, y), UnitSize<BoardSize>, UnitSize<BoardSize>);
}

template <int64_t BoardSize>
QColor
DotCanvas<BoardSize>::Color() const {
  static QColor DarkThemeColor = QColor(202, 202, 202, 255);
  static QColor LightThemeColor = QColor(255, 255, 255, 255);

  return isDarkTheme() ? DarkThemeColor : LightThemeColor;
}

}  // namespace dab::frontend

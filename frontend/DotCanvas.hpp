#pragma once

#include "BaseCanvas.hpp"

namespace dab::detail::frontend {

template <int64_t BoardSize>
class DotCanvas final : public BaseCanvas<BoardSize> {
  public:
  static constexpr int Width = 2 * BaseCanvas<BoardSize>::UnitSize;

  explicit DotCanvas(QPointer<QWidget> parent);

  protected:
  void
  paintEvent(QPaintEvent* event) override;

  private:
  QColor
  Color() const;
};

template <int64_t BoardSize>
DotCanvas<BoardSize>::DotCanvas(QPointer<QWidget> parent) : BaseCanvas<BoardSize>(parent) {
  BaseCanvas<BoardSize>::setFixedSize(Width, Width);
}

template <int64_t BoardSize>
void
DotCanvas<BoardSize>::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setBrush(QBrush(Color()));
  painter.setPen(Qt::NoPen);

  const int x = BaseCanvas<BoardSize>::width() / 2;
  const int y = BaseCanvas<BoardSize>::height() / 2;
  painter.drawEllipse(QPoint(x, y), BaseCanvas<BoardSize>::UnitSize, BaseCanvas<BoardSize>::UnitSize);
}

template <int64_t BoardSize>
QColor
DotCanvas<BoardSize>::Color() const {
  static constexpr QColor DarkThemeColor = QColor(202, 202, 202, 255);
  static constexpr QColor LightThemeColor = QColor(255, 255, 255, 255);

  return BaseCanvas<BoardSize>::ThemeColor(DarkThemeColor, LightThemeColor);
}

}  // namespace dab::detail::frontend
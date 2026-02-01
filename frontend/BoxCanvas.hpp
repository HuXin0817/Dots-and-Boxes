#pragma once

#include "EdgeCanvas.hpp"

namespace dab::detail::frontend {

template <int64_t BoardSize>
class BoxCanvas final : public BaseCanvas<BoardSize> {
  public:
  static constexpr int Width = EdgeCanvas<BoardSize>::Height - 2 * BaseCanvas<BoardSize>::UnitSize;

  explicit BoxCanvas(QPointer<QWidget> parent);

  protected:
  void
  paintEvent(QPaintEvent* event) override;

  private:
  QColor
  Color() const;
};

template <int64_t BoardSize>
BoxCanvas<BoardSize>::BoxCanvas(QPointer<QWidget> parent) : BaseCanvas<BoardSize>(parent) {
  BaseCanvas<BoardSize>::setFixedSize(Width, Width);
}

template <int64_t BoardSize>
void
BoxCanvas<BoardSize>::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::NoPen);
  painter.setBrush(QBrush(Color()));
  painter.drawRect(BaseCanvas<BoardSize>::rect());
}

template <int64_t BoardSize>
QColor
BoxCanvas<BoardSize>::Color() const {
  static const QColor Player1OccupyColor = QColor(64, 64, 255, 64);
  static const QColor Player2OccupyColor = QColor(255, 64, 64, 64);

  if (BaseCanvas<BoardSize>::Owner.has_value()) {
    if (BaseCanvas<BoardSize>::Owner->IsPlayer1Turn()) {
      return Player1OccupyColor;
    }
    if (BaseCanvas<BoardSize>::Owner->IsPlayer2Turn()) {
      return Player2OccupyColor;
    }
  }
  return QColor(0, 0, 0, 0);
}

}  // namespace dab::detail::frontend
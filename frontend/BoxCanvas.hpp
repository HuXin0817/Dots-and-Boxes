#pragma once

#include "Common.hpp"
#include "EdgeCanvas.hpp"

namespace dab::frontend {

template <int64_t BoardSize>
class BoxCanvas final : public QWidget {
  public:
  static constexpr int Width = EdgeCanvas<BoardSize>::Height - 2 * UnitSize<BoardSize>;

  explicit BoxCanvas(QPointer<QWidget> parent);

  void
  SetState(const Turn turn);

  protected:
  void
  paintEvent(QPaintEvent* event) override;

  private:
  State State = State::Free;

  QColor
  Color() const;
};

template <int64_t BoardSize>
BoxCanvas<BoardSize>::BoxCanvas(QPointer<QWidget> parent) : QWidget(parent) {
  setFixedSize(QSize(Width, Width));
}

template <int64_t BoardSize>
void
BoxCanvas<BoardSize>::SetState(const Turn turn) {
  State = StateFromTurn(turn);
}

template <int64_t BoardSize>
void
BoxCanvas<BoardSize>::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::NoPen);
  painter.setBrush(QBrush(Color()));

  painter.drawRect(rect());
}

template <int64_t BoardSize>
QColor
BoxCanvas<BoardSize>::Color() const {
  static QColor Player1OccupyColor = {64, 64, 255, 64};
  static QColor Player2OccupyColor = {255, 64, 64, 64};

  if (State == State::Player1Occupy) {
    return Player1OccupyColor;
  }
  if (State == State::Player2Occupy) {
    return Player2OccupyColor;
  }
  return {0, 0, 0, 0};
}

}  // namespace dab::frontend

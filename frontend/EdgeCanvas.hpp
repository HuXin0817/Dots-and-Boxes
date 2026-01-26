#pragma once

#include "Common.hpp"

namespace dab::frontend {

template <int64_t BoardSize>
class EdgeCanvas final : public QWidget {
  public:
  static constexpr int Width = UnitSize<BoardSize> * 2;
  static constexpr int Height = Width * 5;

  explicit EdgeCanvas(const bool rotate, const std::function<void()>& callBack, QPointer<QWidget> parent);

  void
  SetState(const Turn turn);
  void
  SetHighLight(const bool highLight);

  protected:
  void
  mousePressEvent(QMouseEvent* event) override;
  void
  paintEvent(QPaintEvent* event) override;
  void
  enterEvent(QEnterEvent* event) override;
  void
  leaveEvent(QEvent* event) override;

  private:
  bool Hovered = false;
  bool HighLight = true;
  State State = State::Free;
  const std::function<void()> CallBack;

  QColor
  Color() const;
};

template <int64_t BoardSize>
EdgeCanvas<BoardSize>::EdgeCanvas(const bool rotate, const std::function<void()>& callBack, QPointer<QWidget> parent)
    : QWidget(parent), CallBack(callBack) {
  setFixedSize(QSize(rotate ? Width : Height, rotate ? Height : Width));
}

template <int64_t BoardSize>
void
EdgeCanvas<BoardSize>::SetState(const Turn turn) {
  State = StateFromTurn(turn);
}

template <int64_t BoardSize>
void
EdgeCanvas<BoardSize>::SetHighLight(const bool highLight) {
  HighLight = highLight;
}

template <int64_t BoardSize>
void
EdgeCanvas<BoardSize>::mousePressEvent(QMouseEvent* event) {
  QWidget::mousePressEvent(event);

  CallBack();
}

template <int64_t BoardSize>
void
EdgeCanvas<BoardSize>::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::NoPen);
  painter.setBrush(QBrush(Color()));

  painter.drawRect(rect());
}

template <int64_t BoardSize>
void
EdgeCanvas<BoardSize>::enterEvent(QEnterEvent* event) {
  QWidget::enterEvent(event);
  Hovered = true;
  update();
}

template <int64_t BoardSize>
void
EdgeCanvas<BoardSize>::leaveEvent(QEvent* event) {
  QWidget::leaveEvent(event);
  Hovered = false;
  update();
}

template <int64_t BoardSize>
QColor
EdgeCanvas<BoardSize>::Color() const {
  static QColor DarkThemeColor = {65, 65, 65, 255};
  static QColor LightThemeColor = {217, 217, 217, 255};
  static QColor DarkThemeHoveredColor = {90, 90, 90, 255};
  static QColor LightThemeHoveredColor = {202, 202, 202, 255};
  static QColor Player1OccupyColor = {64, 64, 255, 255};
  static QColor Player2OccupyColor = {255, 64, 64, 255};

  if (State == State::Free) {
    if (Hovered) {
      return isDarkTheme() ? DarkThemeHoveredColor : LightThemeHoveredColor;
    }
    return isDarkTheme() ? DarkThemeColor : LightThemeColor;
  }

  QColor color;
  if (State == State::Player1Occupy) {
    color = Player1OccupyColor;
  } else if (State == State::Player2Occupy) {
    color = Player2OccupyColor;
  }

  if (HighLight) {
    color.setAlpha(255);
  } else if (Hovered) {
    color.setAlpha(144);
  } else {
    color.setAlpha(128);
  }

  return color;
}

}  // namespace dab::frontend

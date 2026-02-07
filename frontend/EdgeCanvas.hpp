#pragma once

#include "BaseCanvas.hpp"

namespace dab::detail::frontend {

template <int64_t BoardSize>
class EdgeCanvas final : public BaseCanvas<BoardSize> {
 public:
  static constexpr int Width = BaseCanvas<BoardSize>::UnitSize * 2;
  static constexpr int Height = Width * 5;

  explicit EdgeCanvas(const bool rotate, const std::function<void()>& callBack, QWidget* parent);

  void
  SetHighLight(const bool highLight);

 protected:
  void
  mousePressEvent(QMouseEvent* event) override;
  void
  paintEvent(QPaintEvent* event) override;

 private:
  bool HighLight = true;
  const std::function<void()> CallBack;

  QColor
  Color() const;
};

template <int64_t BoardSize>
EdgeCanvas<BoardSize>::EdgeCanvas(const bool rotate, const std::function<void()>& callBack, QWidget* parent)
    : BaseCanvas<BoardSize>(parent), CallBack(callBack) {
  BaseCanvas<BoardSize>::setFixedSize(rotate ? Width : Height, rotate ? Height : Width);
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
  painter.drawRect(BaseCanvas<BoardSize>::rect());
}

template <int64_t BoardSize>
QColor
EdgeCanvas<BoardSize>::Color() const {
  static constexpr QColor DarkThemeColor = QColor(65, 65, 65, 255);
  static constexpr QColor LightThemeColor = QColor(217, 217, 217, 255);
  static constexpr QColor DarkThemeHoveredColor = QColor(90, 90, 90, 255);
  static constexpr QColor LightThemeHoveredColor = QColor(202, 202, 202, 255);
  static constexpr QColor Player1OccupyColor = QColor(64, 64, 255, 255);
  static constexpr QColor Player2OccupyColor = QColor(255, 64, 64, 255);

  if (BaseCanvas<BoardSize>::GetOwner() == Owner::None) {
    if (BaseCanvas<BoardSize>::Hovered()) {
      return BaseCanvas<BoardSize>::ThemeColor(DarkThemeHoveredColor, LightThemeHoveredColor);
    }
    return BaseCanvas<BoardSize>::ThemeColor(DarkThemeColor, LightThemeColor);
  }

  QColor color;
  if (BaseCanvas<BoardSize>::GetOwner() == Owner::Player1) {
    color = Player1OccupyColor;
  } else if (BaseCanvas<BoardSize>::GetOwner() == Owner::Player2) {
    color = Player2OccupyColor;
  }
  if (HighLight) {
    color.setAlpha(255);
  } else if (BaseCanvas<BoardSize>::Hovered()) {
    color.setAlpha(144);
  } else {
    color.setAlpha(128);
  }

  return color;
}

}  // namespace dab::detail::frontend
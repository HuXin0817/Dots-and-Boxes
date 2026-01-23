#pragma once

#include "BaseCanvas.hpp"

template <int64_t BoardSize>
class EdgeCanvas : public BaseCanvas<BoardSize> {
  using Base = BaseCanvas<BoardSize>;

  public:
  static constexpr int Width = Base::UnitSize * 2;
  static constexpr int Height = Width * 5;

  explicit EdgeCanvas(const bool rotate, const std::function<void()>& callBack, QWidget* parent)
      : Base(parent), CallBack(callBack) {
    Base::resize(QSize(rotate ? Width : Height, rotate ? Height : Width));
  }

  void
  mousePressEvent(QMouseEvent* event) override {
    BaseCanvas<BoardSize>::mousePressEvent(event);

    CallBack();
  }

  QColor
  Color() const {
    static QColor DarkThemeColor = {65, 65, 65, 255};
    static QColor LightThemeColor = {217, 217, 217, 255};
    static QColor DarkThemeHoveredColor = {90, 90, 90, 255};
    static QColor LightThemeHoveredColor = {202, 202, 202, 255};
    static QColor Player1OccupyColor = {64, 64, 255, 255};
    static QColor Player2OccupyColor = {255, 64, 64, 255};

    if (BaseCanvas<BoardSize>::State == Base::State::Free) {
      if (Base::Hovered) {
        return Base::isDarkTheme() ? DarkThemeHoveredColor : LightThemeHoveredColor;
      }
      return Base::isDarkTheme() ? DarkThemeColor : LightThemeColor;
    }

    QColor color;
    if (BaseCanvas<BoardSize>::State == Base::State::Player1Occupy) {
      color = Player1OccupyColor;
    } else if (BaseCanvas<BoardSize>::State == Base::State::Player2Occupy) {
      color = Player2OccupyColor;
    }

    if (HighLight) {
      color.setAlpha(255);
    } else {
      color.setAlpha(128);
    }

    return color;
  }

  void
  SetHighLight(const bool highLight) {
    HighLight = highLight;
  }

  protected:
  void
  paintEvent(QPaintEvent* event) override {
    Base::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Color()));

    painter.drawRect(Base::rect());
  }

  private:
  bool HighLight = true;
  const std::function<void()> CallBack;
};

#pragma once

#include "BaseCanvas.hpp"
#include "BaseEdgeCanvas.hpp"

template <int64_t BoardSize>
class EdgeCanvas final : public BaseEdgeCanvas<BoardSize> {
  using Base = BaseEdgeCanvas<BoardSize>;

  public:
  explicit EdgeCanvas(const bool rotate, QWidget* parent) : Base(rotate, parent) {
  }

  QColor
  Color() const override {
    static QColor DarkThemeColor = {65, 65, 65, 255};
    static QColor LightThemeColor = {217, 217, 217, 255};
    static QColor Player1OccupyColor = {64, 64, 255, 255};
    static QColor Player2OccupyColor = {255, 64, 64, 255};

    if (BaseCanvas<BoardSize>::State == Base::State::Free) {
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

    painter.drawRect(Base::width() / 2 - Base::RotateWidth() / 2,
                     Base::height() / 2 - Base::RotateHeight() / 2,
                     Base::RotateWidth(),
                     Base::RotateHeight());
  }

  private:
  bool HighLight = true;
};

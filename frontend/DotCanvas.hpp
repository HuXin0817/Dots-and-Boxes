#pragma once

#include "Common.hpp"

template <int BoardSize>
class DotCanvas final : public QWidget {
  public:
  static constexpr int Width = 2 * UnitSize<BoardSize>;

  explicit DotCanvas(QWidget* parent) : QWidget(parent) {
    setFixedSize(Width, Width);
  }

  protected:
  void
  paintEvent(QPaintEvent* event) override {
    QWidget::paintEvent(event);

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(Color()));
    painter.setPen(Qt::NoPen);

    const int x = width() / 2;
    const int y = height() / 2;

    painter.drawEllipse(QPoint(x, y), UnitSize<BoardSize>, UnitSize<BoardSize>);
  }

  private:
  QColor
  Color() const {
    static QColor DarkThemeColor = {202, 202, 202, 255};
    static QColor LightThemeColor = {255, 255, 255, 255};

    return isDarkTheme() ? DarkThemeColor : LightThemeColor;
  }
};

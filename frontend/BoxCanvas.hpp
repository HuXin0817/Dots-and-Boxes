#pragma once

#include "Common.hpp"
#include "EdgeCanvas.hpp"

template <int64_t BoardSize>
class MainWindow;

template <int64_t BoardSize>
class BoxCanvas final : public QWidget {
  friend class MainWindow<BoardSize>;

  public:
  static constexpr int Width = EdgeCanvas<BoardSize>::Height - 2 * UnitSize<BoardSize>;

  explicit BoxCanvas(QWidget* parent) : QWidget(parent) {
    setFixedSize(QSize(Width, Width));
  }

  QColor
  Color() const {
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

  protected:
  void
  paintEvent(QPaintEvent* event) override {
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Color()));

    painter.drawRect(rect());
  }

  private:
  State State = State::Free;
};

#pragma once

#include <QApplication>
#include <QColor>
#include <QWidget>

#include "../../src/model/Turn.hpp"

template <int BoardSize>
class BaseCanvas : public QWidget {
  public:
  static constexpr int UnitSize = 6 + 16 / BoardSize;

  explicit BaseCanvas(QWidget* parent) : QWidget(parent) {
  }

  virtual QColor
  Color() const {
    return {};
  }

  enum class CanvasState {
    Free,
    Player1Occupy,
    Player2Occupy,
  };

  static CanvasState
  StateFromTurn(PlayerTurn turn) {
    return turn.Bool() == Player1Turn.Bool() ? CanvasState::Player1Occupy
                                             : CanvasState::Player2Occupy;
  }

  static bool
  isDarkTheme() {
    auto palette = QApplication::palette();
    auto windowColor = palette.color(QPalette::Window);

    return windowColor.lightness() < 128;
  }
};

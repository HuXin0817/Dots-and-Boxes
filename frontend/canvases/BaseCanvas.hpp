#pragma once

#include <QApplication>
#include <QColor>
#include <QWidget>

#include "../../src/model/Turn.hpp"

namespace dab::frontend::canvas {

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

  static bool
  isDarkTheme() {
    return QApplication::palette().color(QPalette::Window).lightness() < 128;
  }

  void
  SetState(const model::PlayerTurn turn) {
    if (turn.Value() == model::Player1Turn.Value()) {
      State = CanvasState::Player1Occupy;
    } else {
      State = CanvasState::Player2Occupy;
    }
  }

  protected:
  typename BaseCanvas<BoardSize>::CanvasState State = BaseCanvas<BoardSize>::CanvasState::Free;
};

}  // namespace dab::frontend::canvas

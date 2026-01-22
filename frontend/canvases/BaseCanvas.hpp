#pragma once

#include <QApplication>
#include <QWidget>

#include "../../src/model/Turn.hpp"

namespace dab::frontend::canvas {

template <int BoardSize, typename SizeType>
class BaseCanvas : public QWidget {
  public:
  static constexpr int UnitSize = 6 + 16 / BoardSize;

  explicit BaseCanvas(QWidget* parent) : QWidget(parent) {
  }

  virtual QColor
  Color() const {
    return {};
  }

  enum class State {
    Free,
    Player1Occupy,
    Player2Occupy,
  };

  static bool
  isDarkTheme() {
    return QApplication::palette().color(QPalette::Window).lightness() < 128;
  }

  void
  SetState(const model::Turn turn) {
    if (turn.Value() == model::Player1Turn.Value()) {
      State = State::Player1Occupy;
    } else {
      State = State::Player2Occupy;
    }
  }

  protected:
  State State = State::Free;
};

}  // namespace dab::frontend::canvas

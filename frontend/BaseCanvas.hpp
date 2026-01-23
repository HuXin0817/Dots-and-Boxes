#pragma once

#include "../src/model/Turn.hpp"

template <int64_t BoardSize>
class BaseCanvas : public QWidget {
  public:
  static constexpr int UnitSize = 6 + 16 / BoardSize;

  explicit BaseCanvas(QWidget* parent) : QWidget(parent) {
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
  SetState(const Turn turn) {
    if (turn.Value() == Player1Turn.Value()) {
      State = State::Player1Occupy;
    } else {
      State = State::Player2Occupy;
    }
  }

  protected:
  State State = State::Free;
};

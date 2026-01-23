#pragma once

#include "../src/model/Turn.hpp"

enum class State {
  Free,
  Player1Occupy,
  Player2Occupy,
};

static void
SetState(State& State, const Turn turn) {
  if (turn.Value() == Player1Turn.Value()) {
    State = State::Player1Occupy;
  } else {
    State = State::Player2Occupy;
  }
}

static bool
isDarkTheme() {
  return QApplication::palette().color(QPalette::Window).lightness() < 128;
}

template <int64_t BoardSize>
static constexpr int UnitSize = 6 + 16 / BoardSize;

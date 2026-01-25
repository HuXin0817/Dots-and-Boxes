#pragma once

#include <QApplication>
#include <QPainter>
#include <QPalette>
#include <QWidget>

#include "../src/model/Turn.hpp"

namespace dab::frontend {

template <int64_t BoardSize>
static constexpr int UnitSize = 6 + 16 / BoardSize;

enum class State {
  Free,
  Player1Occupy,
  Player2Occupy,
};

inline State
StateFromTurn(const Turn turn) {
  if (turn.IsPlayer1Turn()) {
    return State::Player1Occupy;
  } else {
    return State::Player2Occupy;
  }
}

inline bool
isDarkTheme() {
  return QApplication::palette().color(QPalette::Window).lightness() < 128;
}

}  // namespace dab::frontend

#pragma once

#include <QMainWindow>

#include "../PlayerType.h"

namespace dab {

template <std::int64_t BoardSize>
QMainWindow* CreateMainWindowImpl(PlayerType player1Type, PlayerType player2Type);

template <std::int64_t BoardSize>
QMainWindow* CreateMainWindow(std::int64_t boardSize, PlayerType player1Type, PlayerType player2Type) {
  if constexpr (BoardSize == 0) {
    return nullptr;
  } else {
    if (boardSize < BoardSize) {
      return CreateMainWindow<BoardSize - 1>(boardSize, player1Type, player2Type);
    }
    return CreateMainWindowImpl<BoardSize>(player1Type, player2Type);
  }
}

}  // namespace dab
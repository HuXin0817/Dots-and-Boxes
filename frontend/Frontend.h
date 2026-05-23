#pragma once

#include <QWidget>

#include "../src/PlayerType.h"

namespace dab {

template <std::int64_t BoardSize>
QWidget* CreateMainWindowImpl(PlayerType player1Type, PlayerType player2Type);

template <std::int64_t BoardSize>
QWidget* CreateMainWindow(std::int64_t boardSize, PlayerType player1Type, PlayerType player2Type) {
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
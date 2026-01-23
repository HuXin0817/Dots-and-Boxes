#pragma once

#include "MainWindow.hpp"

class MainWindowCreator {
  template <int64_t N>
  QWidget*
  CreateMainWindowImpl(const int64_t boardSize, const PlayerType player1Type, const PlayerType player2Type) {
    if (boardSize == N) {
      return new MainWindow<N>(player1Type, player2Type);
    }
    if constexpr (N > MinBoardSize) {
      return CreateMainWindowImpl<N - 1>(boardSize, player1Type, player2Type);
    }
    return nullptr;
  }

  public:
  static constexpr int64_t MinBoardSize = 2;
  static constexpr int64_t MaxBoardSize = 36;

  QWidget*
  CreateMainWindow(const int64_t boardSize, const PlayerType player1Type, const PlayerType player2Type) {
    if (boardSize < MinBoardSize || boardSize > MaxBoardSize) {
      return nullptr;
    }
    return CreateMainWindowImpl<MaxBoardSize>(boardSize, player1Type, player2Type);
  }
};

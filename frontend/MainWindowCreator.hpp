#pragma once

#include "MainWindow.hpp"

class MainWindowCreator {
  static constexpr int MaxBoardSize = 36;

  template <int N>
  QWidget*
  CreateMainWindowImpl(int size, PlayerType player1Type, PlayerType player2Type) {
    if (size == N) {
      return new MainWindow<N>(player1Type, player2Type);
    }
    if constexpr (N > 2) {
      return CreateMainWindowImpl<N - 1>(size, player1Type, player2Type);
    }
    return nullptr;
  }

  public:
  QWidget*
  CreateMainWindow(int boardSize, PlayerType player1Type, PlayerType player2Type) {
    if (boardSize <= 1 || boardSize > MaxBoardSize) {
      return nullptr;
    }
    return CreateMainWindowImpl<MaxBoardSize>(boardSize, player1Type, player2Type);
  }
};
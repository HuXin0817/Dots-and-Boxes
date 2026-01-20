#pragma once

#include "MainWindow.hpp"

namespace dab::frontend {

class MainWindowCreator {
  template <int N>
  QWidget*
  CreateMainWindowImpl(const int size, const PlayerType player1Type, const PlayerType player2Type) {
    if (size == N) {
      return new MainWindow<N>(player1Type, player2Type);
    }
    if constexpr (N > 2) {
      return CreateMainWindowImpl<N - 1>(size, player1Type, player2Type);
    }
    return nullptr;
  }

  public:
  static constexpr int MaxBoardSize = 36;

  QWidget*
  CreateMainWindow(const int boardSize, const PlayerType player1Type, const PlayerType player2Type) {
    if (boardSize <= 1 || boardSize > MaxBoardSize) {
      return nullptr;
    }
    return CreateMainWindowImpl<MaxBoardSize>(boardSize, player1Type, player2Type);
  }
};

}  // namespace dab::frontend

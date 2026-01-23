#pragma once

#include "MainWindow.hpp"

class MainWindowCreator {
  template <int N>
  QWidget*
  CreateMainWindowImpl(const int boardSize,
                       const PlayerType player1Type,
                       const PlayerType player2Type,
                       QWidget* parent) {
    if (boardSize == N) {
      return new MainWindow<N>(player1Type, player2Type, parent);
    }
    if constexpr (N > MinBoardSize) {
      return CreateMainWindowImpl<N - 1>(boardSize, player1Type, player2Type, parent);
    }
    return nullptr;
  }

  public:
  static constexpr int MinBoardSize = 2;
  static constexpr int MaxBoardSize = 36;

  QWidget*
  CreateMainWindow(const int boardSize,
                   const PlayerType player1Type,
                   const PlayerType player2Type,
                   QWidget* parent = nullptr) {
    if (boardSize < MinBoardSize || boardSize > MaxBoardSize) {
      return nullptr;
    }
    return CreateMainWindowImpl<MaxBoardSize>(boardSize, player1Type, player2Type, parent);
  }
};

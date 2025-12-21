#pragma once

#include <QWidget>
#include <string>

#include "MainWindow.hpp"

class MainWindowCreator {
  static constexpr int MaxBoardSize = 36;

  template <int N>
  QWidget*
  CreateMainWindowImpl(int size,
                       PlayerType Player1Type,
                       PlayerType Player2Type,
                       RobotType Robot1Type,
                       RobotType Robot2Type) {
    if (size == N) {
      return new MainWindow<N>(Player1Type, Player2Type, Robot1Type, Robot2Type);
    }
    if constexpr (N > 2) {
      return CreateMainWindowImpl<N - 1>(size, Player1Type, Player2Type, Robot1Type, Robot2Type);
    }
    return nullptr;
  }

  public:
  QWidget*
  CreateMainWindow(int BoardSize,
                   PlayerType Player1Type,
                   PlayerType Player2Type,
                   RobotType Robot1Type,
                   RobotType Robot2Type) {
    if (BoardSize <= 1 || BoardSize > MaxBoardSize) {
      return nullptr;
    }
    return CreateMainWindowImpl<MaxBoardSize>(
        BoardSize, Player1Type, Player2Type, Robot1Type, Robot2Type);
  }
};
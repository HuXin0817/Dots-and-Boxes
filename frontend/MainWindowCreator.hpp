#pragma once

#include "MainWindow.hpp"

class MainWindowCreator {
  template <int N>
  QWidget*
  CreateMainWindowImpl(const int boardSize, const PlayerType player1Type, const PlayerType player2Type) {
    if (boardSize == N) {
      static constexpr std::int64_t MaxValue = Edge<N, std::int64_t>::Max;
      if constexpr (MaxValue < std::numeric_limits<std::int8_t>::max()) {
        return new MainWindow<N, std::int8_t>(player1Type, player2Type);
      }
      if constexpr (MaxValue < std::numeric_limits<std::int16_t>::max()) {
        return new MainWindow<N, std::int16_t>(player1Type, player2Type);
      }
      if constexpr (MaxValue < std::numeric_limits<std::int32_t>::max()) {
        return new MainWindow<N, std::int32_t>(player1Type, player2Type);
      }
      return new MainWindow<N, std::int64_t>(player1Type, player2Type);
    }
    if constexpr (N > MinBoardSize) {
      return CreateMainWindowImpl<N - 1>(boardSize, player1Type, player2Type);
    }
    return nullptr;
  }

  public:
  static constexpr int MinBoardSize = 2;
  static constexpr int MaxBoardSize = 36;

  QWidget*
  CreateMainWindow(const int boardSize, const PlayerType player1Type, const PlayerType player2Type) {
    if (boardSize < MinBoardSize || boardSize > MaxBoardSize) {
      return nullptr;
    }
    return CreateMainWindowImpl<MaxBoardSize>(boardSize, player1Type, player2Type);
  }
};

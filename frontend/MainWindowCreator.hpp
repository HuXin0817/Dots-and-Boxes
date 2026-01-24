#pragma once

#include "MainWindow.hpp"

class MainWindowCreator {
  template <int64_t BoardSize>
  QWidget*
  CreateMainWindowImpl(const int64_t boardSize,
                       const PlayerType player1Type,
                       const PlayerType player2Type,
                       QWidget* parent);

  public:
  static constexpr int64_t MinBoardSize = 2;
  static constexpr int64_t MaxBoardSize = 36;

  QWidget*
  CreateMainWindow(const int64_t boardSize,
                   const PlayerType player1Type,
                   const PlayerType player2Type,
                   QWidget* parent = nullptr);
};

template <int64_t BoardSize>
QWidget*
MainWindowCreator::CreateMainWindowImpl(const int64_t boardSize,
                                        const PlayerType player1Type,
                                        const PlayerType player2Type,
                                        QWidget* parent) {
  if (boardSize == BoardSize) {
    return new MainWindow<BoardSize>(player1Type, player2Type, parent);
  }
  if constexpr (BoardSize > MinBoardSize) {
    return CreateMainWindowImpl<BoardSize - 1>(boardSize, player1Type, player2Type, parent);
  }
  return nullptr;
}

inline QWidget*
MainWindowCreator::CreateMainWindow(const int64_t boardSize,
                                    const PlayerType player1Type,
                                    const PlayerType player2Type,
                                    QWidget* parent) {
  if (boardSize < MinBoardSize || boardSize > MaxBoardSize) {
    return nullptr;
  }
  return CreateMainWindowImpl<MaxBoardSize>(boardSize, player1Type, player2Type, parent);
}

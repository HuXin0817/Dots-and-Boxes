#pragma once

#include <tbb/concurrent_queue.h>

#include <QApplication>
#include <QStyleHints>
#include <QThread>
#include <QWidget>

#include "../src/Robot.h"

namespace dab::__detail__::frontend {

class GlobalEnv {
 public:
  GlobalEnv() { HumanMoveEdgeQueue.set_capacity(1); }

  bool IsDarkTheme() const { return QApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark; }

  QColor ThemeColor(const QColor& darkThemeColor, const QColor& lightThemeColor) const {
    return IsDarkTheme() ? darkThemeColor : lightThemeColor;
  }

  int GetUnitSize() const { return UnitSize; }

  void ResetUnitSize() { UnitSize = DefaultUnitSize; }

  void AddUnitSize();

  void ReduceUnitSize();

  const GameBoard& GetBoard() const { return Board; }

  GameBoard& GetBoard() { return Board; }

  void SetPlayer1Type(PlayerType player1Type) { Player1Type = player1Type; }

  void SetPlayer2Type(PlayerType player2Type) { Player2Type = player2Type; }

  PlayerType GetPlayer1Type() const { return Player1Type; }

  PlayerType GetPlayer2Type() const { return Player2Type; }

  void SetHumanMoveEdge(Edge edge);

  Edge GetHumanMoveEdgeSync();

 private:
  static constexpr int DefaultUnitSize = 6 + 16 / BoardSize;
  int UnitSize = DefaultUnitSize;

  PlayerType Player1Type;
  PlayerType Player2Type;
  tbb::concurrent_bounded_queue<Edge> HumanMoveEdgeQueue;
  GameBoard Board;
};

}  // namespace dab::__detail__::frontend
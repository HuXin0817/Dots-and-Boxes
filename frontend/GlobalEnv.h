#pragma once

#include <tbb/concurrent_queue.h>

#include <QApplication>
#include <QStyleHints>
#include <QThread>
#include <QWidget>

#include "../src/Robot.h"

namespace dab::__detail__::frontend {

inline QSize MakeSize(int x) { return {x, x}; }

template <typename Square>
inline QPoint MakePos(Square value, int padding, int width) {
  return {value.X() * width + padding, value.Y() * width + padding};
}

class GlobalEnv {
 public:
  GlobalEnv() { HumanMoveEdgeQueue.set_capacity(1); }

  static constexpr int kDefaultUnitSize = 6 + 16 / BoardSize;

  static constexpr int kDotRadius = 1;
  static constexpr int kDotDiameter = 2 * kDotRadius;
  static constexpr int kEdgeWidth = 5 * kDotDiameter;
  static constexpr int kBoxWidth = kEdgeWidth - 2 * kDotRadius;
  static constexpr int kBoardSize = BoardSize * kEdgeWidth + 2 * kDotRadius;
  static constexpr int kWindowSize = kBoardSize + 2 * kBoxWidth;
  static constexpr int kPadding = (kWindowSize - kBoardSize) / 2;
  static constexpr int kBoxPadding = (kWindowSize - kBoardSize) / 2 + kDotDiameter;

  int DotRadius() const { return kDotRadius * UnitSize; }
  int DotDiameter() const { return kDotDiameter * UnitSize; }
  int EdgeWidth() const { return kEdgeWidth * UnitSize; }
  int BoxWidth() const { return kBoxWidth * UnitSize; }
  int BoardSize() const { return kBoardSize * UnitSize; }
  int WindowSize() const { return kWindowSize * UnitSize; }
  int Padding() const { return kPadding * UnitSize; }
  int BoxPadding() const { return kBoxPadding * UnitSize; }

  bool IsDarkTheme() const { return QApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark; }
  QColor ThemeColor(QColor dark, QColor light) const { return IsDarkTheme() ? dark : light; }
  void ResetUnitSize() { UnitSize = kDefaultUnitSize; }
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
  int UnitSize = kDefaultUnitSize;

  PlayerType Player1Type;
  PlayerType Player2Type;
  tbb::concurrent_bounded_queue<Edge> HumanMoveEdgeQueue;
  GameBoard Board;
};

}  // namespace dab::__detail__::frontend
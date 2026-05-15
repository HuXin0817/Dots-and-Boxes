#pragma once

#include "../Board.h"
#include "ImproveGreedyRobot.h"

namespace dab::__detail__::robot {

class SimulationRobot {
 public:
  SimulationRobot() = default;
  template <typename Board>
  Span<const Edge> BestCandidateEdges(const Board& board);
  List<Edge, Edge::Max>& GetSearchEdges() { return SearchEdges; }

 private:
  ImproveGreedyRobot SubRobot;
  RelativeScoreBoard SimulationBoard;
  List<Edge, Edge::Max> SearchEdges;
};

template <typename Board>
Span<const Edge> SimulationRobot::BestCandidateEdges(const Board& board) {
  if (const Span<const Edge> edges = SubRobot.BestCandidateEdges(board); SubRobot.EnemyUnscoreable()) {
    return edges;
  }

  SearchEdges.Clear();
  Int maxScore = -Box::Max;
  const Int turn = board.GetTurn();
  for (const Edge emptyEdge : board.EmptyEdges()) {
    SimulationBoard = board;
    SimulationBoard.Add(emptyEdge);
    while (SimulationBoard.Gaming()) {
      const Edge edge = SubRobot.SearchOne(SimulationBoard);
      assert(board.MaxEdgeCount(edge) > 1);
      SimulationBoard.Add(edge);
    }
    if (const Int score = turn * SimulationBoard.RelativeScore(); score > maxScore) {
      maxScore = score;
      SearchEdges.ClearAndSet(emptyEdge);
    } else if (score == maxScore) {
      SearchEdges.Append(emptyEdge);
    }
  }

  return SearchEdges;
}

}  // namespace dab::__detail__::robot
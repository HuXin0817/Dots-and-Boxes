#pragma once

#include "Common.hpp"
#include "ImproveGreedyRobot.hpp"

namespace dab::detail::robot {

template <int64_t BoardSize>
class SimulationRobot final : public Robot<BoardSize> {
  public:
  SimulationRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override;

  private:
  ImproveGreedyRobot<BoardSize> SubRobot;
  ScoreCountableBoard<BoardSize> SimulationBoard;
  List<Edge<BoardSize>, Edge<BoardSize>::Max> SearchEdges;
};

template <int64_t BoardSize>
Span<Edge<BoardSize>>
SimulationRobot<BoardSize>::BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) {
  if (Span<Edge<BoardSize>> edges = SubRobot.BestCandidateEdges(board); SubRobot.EnemyUnscoreable()) {
    return edges;
  }

  SearchEdges.Clear();
  Int<BoardSize> maxScore = -Box<BoardSize>::Max;
  for (const Edge<BoardSize> emptyEdge : board.EmptyEdges()) {
    SimulationBoard.Reset(static_cast<EdgeCountableBoard<BoardSize>>(board));
    SimulationBoard.Add(emptyEdge);
    while (SimulationBoard.Gaming()) {
      const Edge<BoardSize> edge = SubRobot.BestCandidateEdges(SimulationBoard).Front();
      assert(board.MaxEdgeCount(edge) > 1);
      SimulationBoard.Add(edge);
    }
    if (const Int<BoardSize> score = SimulationBoard.Score(); score > maxScore) {
      maxScore = score;
      SearchEdges.ClearAndSet(emptyEdge);
    } else if (score == maxScore) {
      SearchEdges.Append(emptyEdge);
    }
  }

  return Span(SearchEdges.Begin(), SearchEdges.End());
}

}  // namespace dab::detail::robot
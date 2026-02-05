#pragma once

#include "ImproveGreedyRobot.hpp"

namespace dab::detail::robot {

template <int64_t BoardSize>
class SimulationRobot final : public Robot<BoardSize> {
  public:
  SimulationRobot() = default;

  Span<Edge<BoardSize>> BestCandidateEdges(const RelativeScoreBoard<BoardSize>& board) override;

  private:
  ImproveGreedyRobot<BoardSize> SubRobot;
  RelativeScoreBoard<BoardSize> SimulationBoard;
  List<Edge<BoardSize>, Edge<BoardSize>::Max> SearchEdges;
};

template <int64_t BoardSize>
Span<Edge<BoardSize>> SimulationRobot<BoardSize>::BestCandidateEdges(const RelativeScoreBoard<BoardSize>& board) {
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
    if (const Int<BoardSize> score = SimulationBoard.RelativeScore(); score > maxScore) {
      maxScore = score;
      SearchEdges.ClearAndSet(emptyEdge);
    } else if (score == maxScore) {
      SearchEdges.Append(emptyEdge);
    }
  }

  return Span(SearchEdges.begin(), SearchEdges.end());
}

}  // namespace dab::detail::robot
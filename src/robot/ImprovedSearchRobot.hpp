#pragma once

#include "../common/List.hpp"
#include "../common/Span.hpp"
#include "BasicSearchRobot.hpp"

namespace dab {

template <int64_t BoardSize>
class ImprovedSearchRobot final : public Robot<BoardSize> {
  public:
  ImprovedSearchRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override;

  private:
  BasicSearchRobot<BoardSize> SubRobot;
  ScoreCountableBoard<BoardSize> SimulationBoard;
  List<Edge<BoardSize>, Edge<BoardSize>::Max> SearchEdges;
};

template <int64_t BoardSize>
Span<Edge<BoardSize>>
ImprovedSearchRobot<BoardSize>::BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) {
  if (Span<Edge<BoardSize>> edges = SubRobot.BestCandidateEdges(board); SubRobot.EnemyUnscoreable()) {
    return edges;
  }

  SearchEdges.Clear();
  SizeType<BoardSize> maxScore = -Box<BoardSize>::Max;
  for (const Edge<BoardSize> emptyEdge : board.EmptyEdges()) {
    SimulationBoard.Reset(static_cast<EdgeCountableBoard<BoardSize>>(board));
    SimulationBoard.Add(emptyEdge);
    while (SimulationBoard.Gaming()) {
      const Edge<BoardSize> edge = SubRobot.BestCandidateEdges(SimulationBoard).At(0);
      assert(board.MaxEdgeCount(edge.Value()) > 1);
      SimulationBoard.Add(edge);
    }
    if (const SizeType<BoardSize> score = SimulationBoard.Score(); score > maxScore) {
      maxScore = score;
      SearchEdges.Reset(emptyEdge);
    } else if (score == maxScore) {
      SearchEdges.Append(emptyEdge);
    }
  }

  return Export(SearchEdges);
}

}  // namespace dab

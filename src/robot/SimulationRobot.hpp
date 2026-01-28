#pragma once

#include "../common/List.hpp"
#include "../common/Span.hpp"
#include "MinimaxRobot.hpp"

namespace dab {

template <int64_t BoardSize>
class SimulationRobot final : public Robot<BoardSize> {
  public:
  SimulationRobot() = default;

  Span<const Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override;

  private:
  MinimaxRobot<BoardSize> SubRobot;
  ScoreCountableBoard<BoardSize> SimulationBoard;
  List<Edge<BoardSize>, Edge<BoardSize>::Max> SearchEdges;
};

template <int64_t BoardSize>
Span<const Edge<BoardSize>>
SimulationRobot<BoardSize>::BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) {
  if (const Span<const Edge<BoardSize>> edges = SubRobot.BestCandidateEdges(board); SubRobot.EnemyUnscoreable()) {
    return edges;
  }

  SearchEdges.Clear();
  SizeType<BoardSize> maxScore = -Box<BoardSize>::Max;
  for (const Edge<BoardSize> emptyEdge : board.EmptyEdges()) {
    SimulationBoard.Reset(static_cast<EdgeCountableBoard<BoardSize>>(board));
    SimulationBoard.Add(emptyEdge);
    while (SimulationBoard.Gaming()) {
      const Edge<BoardSize> edge = SubRobot.BestCandidateEdges(SimulationBoard).At(0);
      assert(board.MaxEdgeCount(edge) > 1);
      SimulationBoard.Add(edge);
    }
    if (const SizeType<BoardSize> score = SimulationBoard.Score(); score > maxScore) {
      maxScore = score;
      SearchEdges.ClearAndSet(emptyEdge);
    } else if (score == maxScore) {
      SearchEdges.Append(emptyEdge);
    }
  }

  return Export<List<Edge<BoardSize>, Edge<BoardSize>::Max>>(SearchEdges);
}

}  // namespace dab

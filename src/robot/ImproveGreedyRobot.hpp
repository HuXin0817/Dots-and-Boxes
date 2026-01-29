#pragma once

#include "Board.hpp"
#include "Common.hpp"
#include "GreedyRobot.hpp"

namespace dab::detail::robot {

template <int64_t BoardSize>
class ImproveGreedyRobot final : public GreedyRobot<BoardSize> {
  public:
  ImproveGreedyRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override;

  private:
  ScoreableEdgeBoard<BoardSize> SimulationBoard;
};

template <int64_t BoardSize>
Span<Edge<BoardSize>>
ImproveGreedyRobot<BoardSize>::BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) {
  if (Span<Edge<BoardSize>> edges = GreedyRobot<BoardSize>::BestCandidateEdges(board);
      GreedyRobot<BoardSize>::EnemyUnscoreable() || GreedyRobot<BoardSize>::Scoreable()) {
    return edges;
  }

  SizeType<BoardSize> minScore = Limits<Box<BoardSize>>::Max + 1;
  Array<Edge<BoardSize>, Limits<Edge<BoardSize>>::Max>& candidateEdges = GreedyRobot<BoardSize>::GetEdgeBuffer();
  SizeType<BoardSize> candidateEdgesSize = 0;

  for (const Edge<BoardSize> edge : board.EmptyEdges()) {
    SimulationBoard.Reset(static_cast<EdgeCountableBoard<BoardSize>>(board));
    SimulationBoard.Add(edge);
    if (const SizeType<BoardSize> score = SimulationBoard.MaxObtainableScore(minScore); score < minScore) {
      minScore = score;
      candidateEdgesSize = 1;
      candidateEdges[0] = edge;
    } else if (score == minScore) {
      candidateEdges[candidateEdgesSize++] = edge;
    }
  }

  return Span(candidateEdges.begin(), candidateEdges.begin() + candidateEdgesSize);
}

}  // namespace dab::detail::robot
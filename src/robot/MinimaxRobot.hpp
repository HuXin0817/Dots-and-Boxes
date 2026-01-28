#pragma once

#include "../board/ScoreableEdgeBoard.hpp"
#include "../common/Span.hpp"
#include "GreedyRobot.hpp"

namespace dab {

template <int64_t BoardSize>
class MinimaxRobot final : public Robot<BoardSize> {
  public:
  MinimaxRobot() = default;

  Span<const Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override;
  bool
  EnemyUnscoreable() const;
  bool
  Scoreable() const;

  private:
  GreedyRobot<BoardSize> SubRobot;
  ScoreableEdgeBoard<BoardSize> SimulationBoard;
};

template <int64_t BoardSize>
Span<const Edge<BoardSize>>
MinimaxRobot<BoardSize>::BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) {
  if (const Span<const Edge<BoardSize>> edges = SubRobot.BestCandidateEdges(board);
      SubRobot.EnemyUnscoreable() || SubRobot.Scoreable()) {
    return edges;
  }

  SizeType<BoardSize> minScore = Box<BoardSize>::Max + 1;
  Array<Edge<BoardSize>, Edge<BoardSize>::Max>& candidateEdges = SubRobot.GetEdgeBuffer();
  SizeType<BoardSize> candidateEdgesSize = 0;

  for (const Edge<BoardSize> edge : board.EmptyEdges()) {
    SimulationBoard.Reset(static_cast<EdgeCountableBoard<BoardSize>>(board));
    SimulationBoard.Add(edge);
    if (const SizeType<BoardSize> score = SimulationBoard.MaxObtainableScore(minScore); score < minScore) {
      minScore = score;
      candidateEdgesSize = 1;
      candidateEdges.At(0) = edge;
    } else if (score == minScore) {
      candidateEdges.At(candidateEdgesSize++) = edge;
    }
  }

  return Span<const Edge<BoardSize>>(candidateEdges.begin(), candidateEdges.begin() + candidateEdgesSize);
}

template <int64_t BoardSize>
bool
MinimaxRobot<BoardSize>::EnemyUnscoreable() const {
  return SubRobot.EnemyUnscoreable();
}

template <int64_t BoardSize>
bool
MinimaxRobot<BoardSize>::Scoreable() const {
  return SubRobot.Scoreable();
}

}  // namespace dab

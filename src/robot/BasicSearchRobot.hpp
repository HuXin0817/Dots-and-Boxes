#pragma once

#include "../board/ScoreableEdgeBoard.hpp"
#include "../common/Span.hpp"
#include "SimpleStrategyRobot.hpp"

namespace dab {

template <int64_t BoardSize>
class BasicSearchRobot final : public SimpleStrategyRobot<BoardSize> {
  public:
  BasicSearchRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override;

  private:
  ScoreableEdgeBoard<BoardSize> SimulationBoard;
};

template <int64_t BoardSize>
Span<Edge<BoardSize>>
BasicSearchRobot<BoardSize>::BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) {
  if (Span<Edge<BoardSize>> edges = SimpleStrategyRobot<BoardSize>::BestCandidateEdges(board);
      SimpleStrategyRobot<BoardSize>::EnemyUnscoreable() || SimpleStrategyRobot<BoardSize>::Scoreable()) {
    return edges;
  }

  SizeType<BoardSize> minScore = Box<BoardSize>::Max + 1;
  SizeType<BoardSize> candidateEdgesSize = 0;

  for (const Edge<BoardSize> edge : board.EmptyEdges()) {
    SimulationBoard.Reset(static_cast<EdgeCountableBoard<BoardSize>>(board));
    SimulationBoard.Add(edge);
    if (const SizeType<BoardSize> score = SimulationBoard.MaxObtainableScore(minScore); score < minScore) {
      minScore = score;
      candidateEdgesSize = 1;
      SimpleStrategyRobot<BoardSize>::Edges.At(0) = edge;
    } else if (score == minScore) {
      SimpleStrategyRobot<BoardSize>::Edges.At(candidateEdgesSize++) = edge;
    }
  }

  return Span(SimpleStrategyRobot<BoardSize>::Edges.begin(),
              SimpleStrategyRobot<BoardSize>::Edges.begin() + candidateEdgesSize);
}

}  // namespace dab

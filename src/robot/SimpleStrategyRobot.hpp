#pragma once

#include "../board/ScoreCountableBoard.hpp"
#include "../common/Array.hpp"
#include "../common/Span.hpp"
#include "Robot.hpp"

namespace dab {

template <int64_t BoardSize>
class SimpleStrategyRobot : public Robot<BoardSize> {
  public:
  SimpleStrategyRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override;
  bool
  EnemyUnscoreable() const;
  bool
  Scoreable() const;

  protected:
  Array<Edge<BoardSize>, Edge<BoardSize>::Max> Edges;

  private:
  SizeType<BoardSize> EnemyUnscoreableIndex;
  SizeType<BoardSize> ScoreableIndex;
};

template <int64_t BoardSize>
Span<Edge<BoardSize>>
SimpleStrategyRobot<BoardSize>::BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) {
  ScoreableIndex = 0;
  EnemyUnscoreableIndex = Edge<BoardSize>::Max;

  const Span<Edge<BoardSize>> emptyEdges = board.EmptyEdges();
  for (const Edge<BoardSize> edge : emptyEdges) {
    if (const uint8_t maxCount = board.MaxEdgeCount(edge); maxCount == 3) {
      Edges.At(ScoreableIndex++) = edge;
    } else if (maxCount < 2) {
      Edges.At(--EnemyUnscoreableIndex) = edge;
    }
  }
  assert(ScoreableIndex <= EnemyUnscoreableIndex);

  if (Scoreable()) {
    return Span(Edges.begin(), Edges.begin() + ScoreableIndex);
  }
  if (EnemyUnscoreable()) {
    return Span(Edges.begin() + EnemyUnscoreableIndex, Edges.end());
  }

  return Span(emptyEdges.begin(), emptyEdges.end());
}

template <int64_t BoardSize>
bool
SimpleStrategyRobot<BoardSize>::EnemyUnscoreable() const {
  return EnemyUnscoreableIndex < Edge<BoardSize>::Max;
}

template <int64_t BoardSize>
bool
SimpleStrategyRobot<BoardSize>::Scoreable() const {
  return ScoreableIndex > 0;
}

}  // namespace dab

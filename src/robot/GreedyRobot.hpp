#pragma once

#include "../board/ScoreCountableBoard.hpp"
#include "../common/Array.hpp"
#include "../common/Span.hpp"
#include "Robot.hpp"

namespace dab {

template <int64_t BoardSize>
class GreedyRobot : public Robot<BoardSize> {
  public:
  GreedyRobot() = default;

  Span<const Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override;
  bool
  EnemyUnscoreable() const;
  bool
  Scoreable() const;
  Array<Edge<BoardSize>, Edge<BoardSize>::Max>&
  GetEdgeBuffer();

  private:
  SizeType<BoardSize> EnemyUnscoreableIndex;
  SizeType<BoardSize> ScoreableIndex;
  Array<Edge<BoardSize>, Edge<BoardSize>::Max> Edges;
};

template <int64_t BoardSize>
Span<const Edge<BoardSize>>
GreedyRobot<BoardSize>::BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) {
  ScoreableIndex = 0;
  EnemyUnscoreableIndex = Edge<BoardSize>::Max;

  const Span<const Edge<BoardSize>> emptyEdges = board.EmptyEdges();
  for (const Edge<BoardSize> edge : emptyEdges) {
    if (const uint8_t maxCount = board.MaxEdgeCount(edge); maxCount == 3) {
      Edges.At(ScoreableIndex++) = edge;
    } else if (maxCount < 2) {
      Edges.At(--EnemyUnscoreableIndex) = edge;
    }
  }
  assert(ScoreableIndex <= EnemyUnscoreableIndex);

  if (Scoreable()) {
    return Span<const Edge<BoardSize>>(Edges.begin(), Edges.begin() + ScoreableIndex);
  }
  if (EnemyUnscoreable()) {
    return Span<const Edge<BoardSize>>(Edges.begin() + EnemyUnscoreableIndex, Edges.end());
  }

  return Span<const Edge<BoardSize>>(emptyEdges.begin(), emptyEdges.end());
}

template <int64_t BoardSize>
bool
GreedyRobot<BoardSize>::EnemyUnscoreable() const {
  return EnemyUnscoreableIndex < Edge<BoardSize>::Max;
}

template <int64_t BoardSize>
bool
GreedyRobot<BoardSize>::Scoreable() const {
  return ScoreableIndex > 0;
}

template <int64_t BoardSize>
Array<Edge<BoardSize>, Edge<BoardSize>::Max>&
GreedyRobot<BoardSize>::GetEdgeBuffer() {
  return Edges;
}

}  // namespace dab

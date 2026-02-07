#pragma once

#include "Robot.hpp"

namespace dab::detail::robot {

template <int64_t BoardSize>
class GreedyRobot : public Robot<BoardSize> {
 public:
  GreedyRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const RelativeScoreBoard<BoardSize>& board) override;
  bool
  EnemyUnscoreable() const;
  bool
  Scoreable() const;

 protected:
  Array<Edge<BoardSize>, Edge<BoardSize>::Max>&
  GetEdgeBuffer();

 private:
  Int<BoardSize> EnemyUnscoreableIndex;
  Int<BoardSize> ScoreableIndex;
  Array<Edge<BoardSize>, Edge<BoardSize>::Max> Edges;
};

template <int64_t BoardSize>
Span<Edge<BoardSize>>
GreedyRobot<BoardSize>::BestCandidateEdges(const RelativeScoreBoard<BoardSize>& board) {
  ScoreableIndex = 0;
  EnemyUnscoreableIndex = Edge<BoardSize>::Max;

  Span<Edge<BoardSize>> emptyEdges = board.EmptyEdges();
  for (const Edge<BoardSize> edge : emptyEdges) {
    if (const uint8_t maxCount = board.MaxEdgeCount(edge); maxCount == 3) {
      Edges[ScoreableIndex++] = edge;
    } else if (maxCount < 2) {
      Edges[--EnemyUnscoreableIndex] = edge;
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

}  // namespace dab::detail::robot
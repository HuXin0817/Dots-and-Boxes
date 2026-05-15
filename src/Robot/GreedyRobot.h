#pragma once

#include "../Model.h"

namespace dab::__detail__::robot {

class GreedyRobot {
 public:
  GreedyRobot() = default;

  bool EnemyUnscoreable() const { return EnemyUnscoreableIndex < Edge::Max; }
  bool Scoreable() const { return ScoreableIndex > 0; }
  template <typename Board>
  Span<const Edge> BestCandidateEdges(const Board& board);
  template <typename Board>
  Edge SearchOne(const Board& board);

 protected:
  Array<Edge, Edge::Max> Edges;

 private:
  Int EnemyUnscoreableIndex = 0;
  Int ScoreableIndex = 0;
};

template <typename Board>
Span<const Edge> GreedyRobot::BestCandidateEdges(const Board& board) {
  ScoreableIndex = 0;
  EnemyUnscoreableIndex = Edge::Max;

  const Span<const Edge> emptyEdges = board.EmptyEdges();
  for (const Edge edge : emptyEdges) {
    if (const std::uint8_t maxCount = board.MaxEdgeCount(edge); maxCount == 3) {
      Edges.At(ScoreableIndex++) = edge;
    } else if (maxCount < 2) {
      Edges.At(--EnemyUnscoreableIndex) = edge;
    }
  }
  assert(ScoreableIndex <= EnemyUnscoreableIndex);
  if (Scoreable()) {
    return {Edges.begin(), ScoreableIndex};
  }
  if (EnemyUnscoreable()) {
    return {Edges.begin() + EnemyUnscoreableIndex, Edges.end()};
  }

  return emptyEdges;
}

template <typename Board>
Edge GreedyRobot::SearchOne(const Board& board) {
  Edge result = Edge::Invalid;
  for (const Edge edge : board.EmptyEdges()) {
    if (const std::uint8_t maxCount = board.MaxEdgeCount(edge); maxCount == 3) {
      return edge;
    } else if (maxCount < 2) {
      result = edge;
    }
  }
  return result;
}

}  // namespace dab::__detail__::robot
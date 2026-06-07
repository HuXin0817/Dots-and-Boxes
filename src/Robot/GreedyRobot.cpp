#include "GreedyRobot.h"

#include "../Board.h"

namespace dab::__detail__::robot {

template <typename Board>
iterable::Span<const model::Edge> GreedyRobot::BestCandidateEdges(const Board& board) {
  ScoreableIndex = 0;
  EnemyUnscoreableIndex = model::Edge::Max;

  const iterable::Span<const model::Edge> emptyEdges = board.EmptyEdges();
  for (const model::Edge edge : emptyEdges) {
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
model::Edge GreedyRobot::SearchOne(const Board& board) {
  model::Edge result = model::Edge::Invalid;
  for (const model::Edge edge : board.EmptyEdges()) {
    if (const std::uint8_t maxCount = board.MaxEdgeCount(edge); maxCount == 3) {
      return edge;
    } else if (maxCount < 2) {
      result = edge;
    }
  }
  return result;
}

template iterable::Span<const model::Edge> GreedyRobot::BestCandidateEdges<board::RelativeScoreBoard>(
    const board::RelativeScoreBoard& board);
template iterable::Span<const model::Edge> GreedyRobot::BestCandidateEdges<board::GameBoard>(
    const board::GameBoard& board);

template model::Edge GreedyRobot::SearchOne<board::RelativeScoreBoard>(const board::RelativeScoreBoard& board);

}  // namespace dab::__detail__::robot
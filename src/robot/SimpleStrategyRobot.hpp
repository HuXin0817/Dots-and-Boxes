#pragma once

#include "../board/ScoreCountableBoard.hpp"
#include "../common/List.hpp"
#include "../common/Span.hpp"
#include "Robot.hpp"

namespace dab {

template <int64_t BoardSize>
class BasicSearchRobot;

template <int64_t BoardSize>
class ImprovedSearchRobot;

template <int64_t BoardSize>
class SimpleStrategyRobot final : public Robot<BoardSize> {
  friend class BasicSearchRobot<BoardSize>;
  friend class ImprovedSearchRobot<BoardSize>;

  public:
  SimpleStrategyRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override;

  private:
  List<Edge<BoardSize>, Edge<BoardSize>::Max> EnemyUnscoreableEdges;
  List<Edge<BoardSize>, Edge<BoardSize>::Max> ScoreableEdges;
};

template <int64_t BoardSize>
Span<Edge<BoardSize>>
SimpleStrategyRobot<BoardSize>::BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) {
  EnemyUnscoreableEdges.Clear();
  ScoreableEdges.Clear();
  const Span<Edge<BoardSize>> emptyEdges = board.EmptyEdges();

  for (const Edge<BoardSize> edge : emptyEdges) {
    if (const uint8_t maxCount = board.MaxEdgeCount(edge); maxCount == 3) {
      ScoreableEdges.Append(edge);
    } else if (maxCount < 2) {
      EnemyUnscoreableEdges.Append(edge);
    }
  }

  if (!ScoreableEdges.Empty()) {
    return Export(ScoreableEdges);
  }
  if (!EnemyUnscoreableEdges.Empty()) {
    return Export(EnemyUnscoreableEdges);
  }

  return {emptyEdges.begin(), emptyEdges.end()};
}

}  // namespace dab

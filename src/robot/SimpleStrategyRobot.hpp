#pragma once

#include "../board/ScoreCountableBoard.hpp"
#include "../common/List.hpp"
#include "../common/Span.hpp"
#include "Robot.hpp"

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

  Span<Edge<BoardSize>, SizeType<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override {
    EnemyUnscoreableEdges.Clear();
    ScoreableEdges.Clear();
    const Span<Edge<BoardSize>, SizeType<BoardSize>> emptyEdges =
        board.GetEdgeCountableBoard().GetBasicBoard().EmptyEdges();

    for (const Edge<BoardSize> edge : emptyEdges) {
      if (const SizeType<BoardSize> maxCount = board.GetEdgeCountableBoard().GetEdgeCountOfBox().MaxCount(edge);
          maxCount == 3) {
        ScoreableEdges.Append(edge);
      } else if (maxCount < 2) {
        EnemyUnscoreableEdges.Append(edge);
      }
    }

    if (!ScoreableEdges.Empty()) {
      return Export<List<Edge<BoardSize>, Edge<BoardSize>::Max, SizeType<BoardSize>>, SizeType<BoardSize>>(
          ScoreableEdges);
    }
    if (!EnemyUnscoreableEdges.Empty()) {
      return Export<List<Edge<BoardSize>, Edge<BoardSize>::Max, SizeType<BoardSize>>, SizeType<BoardSize>>(
          EnemyUnscoreableEdges);
    }

    return {emptyEdges.begin(), emptyEdges.end()};
  }

  private:
  List<Edge<BoardSize>, Edge<BoardSize>::Max, SizeType<BoardSize>> EnemyUnscoreableEdges;
  List<Edge<BoardSize>, Edge<BoardSize>::Max, SizeType<BoardSize>> ScoreableEdges;
};

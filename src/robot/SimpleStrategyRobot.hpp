#pragma once

#include "../board/ScoreCountableBoard.hpp"
#include "../common/List.hpp"
#include "../common/Span.hpp"
#include "Robot.hpp"

template <int BoardSize, typename SizeType>
class BasicSearchRobot;

template <int BoardSize, typename SizeType>
class ImprovedSearchRobot;

template <int BoardSize, typename SizeType>
class SimpleStrategyRobot final : public Robot<BoardSize, SizeType> {
  friend class BasicSearchRobot<BoardSize, SizeType>;
  friend class ImprovedSearchRobot<BoardSize, SizeType>;

  public:
  SimpleStrategyRobot() = default;

  Span<Edge<BoardSize, SizeType>, SizeType>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize, SizeType>& board) override {
    EnemyUnscoreableEdges.Clear();
    ScoreableEdges.Clear();
    const Span<Edge<BoardSize, SizeType>, SizeType> emptyEdges =
        board.GetEdgeCountableBoard().GetBasicBoard().EmptyEdges();

    for (const Edge<BoardSize, SizeType> edge : emptyEdges) {
      if (const SizeType maxCount = board.GetEdgeCountableBoard().GetEdgeCountOfBox().MaxCount(edge); maxCount == 3) {
        ScoreableEdges.Append(edge);
      } else if (maxCount < 2) {
        EnemyUnscoreableEdges.Append(edge);
      }
    }

    if (!ScoreableEdges.Empty()) {
      return Export<List<Edge<BoardSize, SizeType>, Edge<BoardSize, SizeType>::Max, SizeType>, SizeType>(
          ScoreableEdges);
    }
    if (!EnemyUnscoreableEdges.Empty()) {
      return Export<List<Edge<BoardSize, SizeType>, Edge<BoardSize, SizeType>::Max, SizeType>, SizeType>(
          EnemyUnscoreableEdges);
    }

    return {emptyEdges.begin(), emptyEdges.end()};
  }

  private:
  List<Edge<BoardSize, SizeType>, Edge<BoardSize, SizeType>::Max, SizeType> EnemyUnscoreableEdges;
  List<Edge<BoardSize, SizeType>, Edge<BoardSize, SizeType>::Max, SizeType> ScoreableEdges;
};

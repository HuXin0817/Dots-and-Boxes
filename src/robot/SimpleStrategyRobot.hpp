#pragma once

#include "../board/ScoreCountableBoard.hpp"
#include "../common/List.hpp"
#include "../common/Span.hpp"
#include "Robot.hpp"

namespace dab::robot {

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

  common::Span<model::Edge<BoardSize, SizeType>, SizeType>
  BestCandidateEdges(const board::ScoreCountableBoard<BoardSize, SizeType>& board) override {
    EnemyUnscoreableEdges.Clear();
    ScoreableEdges.Clear();
    const common::Span<model::Edge<BoardSize, SizeType>, SizeType> emptyEdges =
        board.GetEdgeCountableBoard().GetBasicBoard().EmptyEdges();

    for (const model::Edge<BoardSize, SizeType> edge : emptyEdges) {
      if (const SizeType maxCount = board.GetEdgeCountableBoard().GetEdgeCountOfBox().MaxCount(edge); maxCount == 3) {
        ScoreableEdges.Append(edge);
      } else if (maxCount < 2) {
        EnemyUnscoreableEdges.Append(edge);
      }
    }

    if (!ScoreableEdges.Empty()) {
      return common::Export<
          common::List<model::Edge<BoardSize, SizeType>, model::Edge<BoardSize, SizeType>::Max, SizeType>,
          SizeType>(ScoreableEdges);
    }
    if (!EnemyUnscoreableEdges.Empty()) {
      return common::Export<
          common::List<model::Edge<BoardSize, SizeType>, model::Edge<BoardSize, SizeType>::Max, SizeType>,
          SizeType>(EnemyUnscoreableEdges);
    }

    return {emptyEdges.begin(), emptyEdges.end()};
  }

  private:
  common::List<model::Edge<BoardSize, SizeType>, model::Edge<BoardSize, SizeType>::Max, SizeType> EnemyUnscoreableEdges;
  common::List<model::Edge<BoardSize, SizeType>, model::Edge<BoardSize, SizeType>::Max, SizeType> ScoreableEdges;
};

}  // namespace dab::robot

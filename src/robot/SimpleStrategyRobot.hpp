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

  common::Span<model::Edge<BoardSize, SizeType>, int>
  BestCandidateEdges(const board::ScoreCountableBoard<BoardSize, SizeType>& board) override {
    EnemyUnscoreableEdges.Clear();
    ScoreableEdges.Clear();
    const common::Span<model::Edge<BoardSize, SizeType>, int> emptyEdges =
        board.GetEdgeCountableBoard().GetBasicBoard().EmptyEdges();

    for (const model::Edge<BoardSize, SizeType> edge : emptyEdges) {
      if (const int maxCount = board.GetEdgeCountableBoard().GetEdgeCountOfBox().MaxCount(edge); maxCount == 3) {
        ScoreableEdges.Append(edge);
      } else if (maxCount < 2) {
        EnemyUnscoreableEdges.Append(edge);
      }
    }

    if (!ScoreableEdges.Empty()) {
      return common::Export<common::List<model::Edge<BoardSize, SizeType>, model::Edge<BoardSize, SizeType>::Max, int>,
                            int>(ScoreableEdges);
    }
    if (!EnemyUnscoreableEdges.Empty()) {
      return common::Export<common::List<model::Edge<BoardSize, SizeType>, model::Edge<BoardSize, SizeType>::Max, int>,
                            int>(EnemyUnscoreableEdges);
    }

    return {emptyEdges.begin(), emptyEdges.end()};
  }

  private:
  common::List<model::Edge<BoardSize, SizeType>, model::Edge<BoardSize, SizeType>::Max, int> EnemyUnscoreableEdges;
  common::List<model::Edge<BoardSize, SizeType>, model::Edge<BoardSize, SizeType>::Max, int> ScoreableEdges;
};

}  // namespace dab::robot

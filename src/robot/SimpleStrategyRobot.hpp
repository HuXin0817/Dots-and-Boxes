#pragma once

#include "../board/ScoreCountableBoard.hpp"
#include "../common/List.hpp"
#include "../common/Span.hpp"
#include "Robot.hpp"

namespace dab::robot {

template <int BoardSize>
class BasicSearchRobot;

template <int BoardSize>
class ImprovedSearchRobot;

template <int BoardSize>
class SimpleStrategyRobot final : public Robot<BoardSize> {
  friend class BasicSearchRobot<BoardSize>;
  friend class ImprovedSearchRobot<BoardSize>;

  public:
  SimpleStrategyRobot() = default;

  common::Span<model::Edge<BoardSize>>
  BestCandidateEdges(const board::ScoreCountableBoard<BoardSize>& board) override {
    EnemyUnscoreableEdges.Clear();
    ScoreableEdges.Clear();
    const common::Span<model::Edge<BoardSize>> emptyEdges = board.GetEdgeCountableBoard().GetBasicBoard().EmptyEdges();

    for (const model::Edge<BoardSize> edge : emptyEdges) {
      if (const int maxCount = board.GetEdgeCountableBoard().GetEdgeCountOfBox().MaxCount(edge); maxCount == 3) {
        ScoreableEdges.Append(edge);
      } else if (maxCount < 2) {
        EnemyUnscoreableEdges.Append(edge);
      }
    }

    if (!ScoreableEdges.Empty()) {
      return common::Export(ScoreableEdges);
    }
    if (!EnemyUnscoreableEdges.Empty()) {
      return common::Export(EnemyUnscoreableEdges);
    }

    return {emptyEdges.begin(), emptyEdges.end()};
  }

  private:
  common::List<model::Edge<BoardSize>, model::Edge<BoardSize>::Max> EnemyUnscoreableEdges;
  common::List<model::Edge<BoardSize>, model::Edge<BoardSize>::Max> ScoreableEdges;
};

}  // namespace dab::robot

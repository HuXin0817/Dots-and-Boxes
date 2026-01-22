#pragma once

#include "../board/ScoreableEdgeBoard.hpp"
#include "../common/Span.hpp"
#include "Robot.hpp"
#include "SimpleStrategyRobot.hpp"

namespace dab::robot {

template <int BoardSize, typename SizeType>
class ImprovedSearchRobot;

template <int BoardSize, typename SizeType>
class BasicSearchRobot final : public Robot<BoardSize, SizeType> {
  friend class ImprovedSearchRobot<BoardSize, SizeType>;

  public:
  common::Span<model::Edge<BoardSize, SizeType>, int>
  BestCandidateEdges(const board::ScoreCountableBoard<BoardSize, SizeType>& board) override {
    if (common::Span<model::Edge<BoardSize, SizeType>, int> edges = SubRobot.BestCandidateEdges(board);
        !SubRobot.EnemyUnscoreableEdges.Empty() || !SubRobot.ScoreableEdges.Empty()) {
      return edges;
    }

    int minScore = model::Box<BoardSize, SizeType>::Max + 1;
    common::List<model::Edge<BoardSize, SizeType>, model::Edge<BoardSize, SizeType>::Max, int>& candidateEdges =
        SubRobot.EnemyUnscoreableEdges;
    assert(candidateEdges.Empty());

    for (const model::Edge<BoardSize, SizeType> edge : board.GetEdgeCountableBoard().GetBasicBoard().EmptyEdges()) {
      SimulationBoard.Reset(board.GetEdgeCountableBoard());
      SimulationBoard.Add(edge);
      if (const int score = SimulationBoard.MaxObtainableScore(minScore); score < minScore) {
        minScore = score;
        candidateEdges.Reset(edge);
      } else if (score == minScore) {
        candidateEdges.Append(edge);
      }
    }

    return common::Export<common::List<model::Edge<BoardSize, SizeType>, model::Edge<BoardSize, SizeType>::Max, int>,
                          int>(candidateEdges);
  }

  private:
  SimpleStrategyRobot<BoardSize, SizeType> SubRobot;
  board::ScoreableEdgeBoard<BoardSize, SizeType> SimulationBoard;
};

}  // namespace dab::robot

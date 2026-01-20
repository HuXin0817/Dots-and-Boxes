#pragma once

#include "../board/ScoreableEdgeBoard.hpp"
#include "../common/Span.hpp"
#include "Robot.hpp"
#include "SimpleStrategyRobot.hpp"

namespace dab::robot {

template <int BoardSize>
class ImprovedSearchRobot;

template <int BoardSize>
class BasicSearchRobot final : public Robot<BoardSize> {
  friend class ImprovedSearchRobot<BoardSize>;

  public:
  common::Span<model::Edge<BoardSize>>
  BestCandidateEdges(const board::ScoreCountableBoard<BoardSize>& board) override {
    if (common::Span<model::Edge<BoardSize>> edges = SubRobot.BestCandidateEdges(board);
        !SubRobot.EnemyUnscoreableEdges.Empty() || !SubRobot.ScoreableEdges.Empty()) {
      return edges;
    }

    int minScore = model::Box<BoardSize>::Max + 1;
    common::List<model::Edge<BoardSize>, model::Edge<BoardSize>::Max>& candidateEdges = SubRobot.EnemyUnscoreableEdges;
    assert(candidateEdges.Empty());

    for (const model::Edge<BoardSize> edge : board.GetEdgeCountableBoard().GetBasicBoard().EmptyEdges()) {
      SimulationBoard.Reset(board.GetEdgeCountableBoard());
      SimulationBoard.Add(edge);
      if (const int score = SimulationBoard.MaxObtainableScore(minScore); score < minScore) {
        minScore = score;
        candidateEdges.Reset(edge);
      } else if (score == minScore) {
        candidateEdges.Append(edge);
      }
    }

    return common::Export(candidateEdges);
  }

  private:
  SimpleStrategyRobot<BoardSize> SubRobot;
  board::ScoreableEdgeBoard<BoardSize> SimulationBoard;
};

}  // namespace dab::robot

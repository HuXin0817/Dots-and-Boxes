#pragma once

#include "../board/ScoreableEdgeBoard.hpp"
#include "../common/Span.hpp"
#include "Robot.hpp"
#include "SimpleStrategyRobot.hpp"

template <int BoardSize>
class BasicSearchRobot final : public Robot<BoardSize> {
  public:
  Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override {
    if (auto edges = SubRobot.BestCandidateEdges(board);
        !SubRobot.EnemyUnscoreableEdges.Empty() || !SubRobot.ScoreableEdges.Empty()) {
      return edges;
    }

    int minScore = Box<BoardSize>::Max + 1;
    auto& candidateEdges = SubRobot.EnemyUnscoreableEdges;
    assert(candidateEdges.Empty());

    for (Edge<BoardSize> edge : board.EmptyEdges()) {
      SimulationBoard.Reset(board.GetEdgeCountableBoard());
      SimulationBoard.Add(edge);
      if (int score = SimulationBoard.MaxObtainableScore(minScore); score < minScore) {
        minScore = score;
        candidateEdges.Reset(edge);
      } else if (score == minScore) {
        candidateEdges.Append(edge);
      }
    }

    return Export(candidateEdges);
  }

  SimpleStrategyRobot<BoardSize> SubRobot;
  ScoreableEdgeBoard<BoardSize> SimulationBoard;
};

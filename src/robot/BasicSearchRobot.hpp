#pragma once

#include "../board/BoardV3.hpp"
#include "../common/Span.hpp"
#include "Robot.hpp"
#include "SimpleStrategyRobot.hpp"

class BasicSearchRobot final : public Robot {
  friend class ImprovedSearchRobot;

  public:
  Span<Edge>
  BestCandidateEdges(const BoardV2& board) override {
    if (auto edges = SubRobot.BestCandidateEdges(board);
        !SubRobot.EnemyUnscoreableEdges.Empty() || !SubRobot.ScoreableEdges.Empty()) {
      return edges;
    }

    int minScore = Box::Max + 1;
    auto& candidateEdges = SubRobot.EnemyUnscoreableEdges;
    assert(candidateEdges.Empty());

    for (Edge edge : board.EmptyEdges()) {
      SimulationBoard.Reset(board.GetBoardV1());
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

  private:
  SimpleStrategyRobot SubRobot;
  BoardV3 SimulationBoard;
};

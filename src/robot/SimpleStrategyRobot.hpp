#pragma once

#include "../board/ScoreCountableBoard.hpp"
#include "../common/List.hpp"
#include "../common/Span.hpp"
#include "Robot.hpp"

class SimpleStrategyRobot final : public Robot {
  friend class BasicSearchRobot;
  friend class ImprovedSearchRobot;

  public:
  SimpleStrategyRobot() = default;

  Span<Edge>
  BestCandidateEdges(const ScoreCountableBoard& board) override {
    EnemyUnscoreableEdges.Clear();
    ScoreableEdges.Clear();
    auto EmptyEdges = board.EmptyEdges();

    for (Edge edge : EmptyEdges) {
      if (int maxCount = board.MaxCount(edge); maxCount == 3) {
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

    return {EmptyEdges.begin(), EmptyEdges.end()};
  }

  private:
  List<Edge, Edge::Max> EnemyUnscoreableEdges;
  List<Edge, Edge::Max> ScoreableEdges;
};

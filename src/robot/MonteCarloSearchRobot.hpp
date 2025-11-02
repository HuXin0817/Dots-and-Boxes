#pragma once

#include "../board/ScoreCountableBoard.hpp"
#include "../common/Random.hpp"
#include "../model/EdgeScoreMap.hpp"
#include "ImprovedSearchRobot.hpp"

class MonteCarloSearchRobot final : public Robot {
  friend class ParallelSearchRobot;

  public:
  static constexpr int SearchTime = 10000;

  MonteCarloSearchRobot() = default;

  Span<Edge>
  BestCandidateEdges(const ScoreCountableBoard& board) override {
    if (auto edges = SubRobot.BestCandidateEdges(board); edges.Size() == 1) {
      return edges;
    }

    SearchResult.Reset();
    int times = SearchTime / board.RemainStep() + 1;
    while (times--) {
      SimulationBoard.Reset(board.GetEdgeCountableBoard());
      Edge edge = RandomChoice(SubRobot.BestCandidateEdges(SimulationBoard));
      SimulationBoard.Add(edge);
      while (SimulationBoard.Gaming()) {
        SimulationBoard.Add(RandomChoice(SubRobot.BestCandidateEdges(SimulationBoard)));
      }
      SearchResult.Add(edge, SimulationBoard.Score());
    }

    return SearchResult.Export();
  }

  private:
  ImprovedSearchRobot SubRobot;
  ScoreCountableBoard SimulationBoard;
  EdgeScoreMap SearchResult;
};

#pragma once

#include "MonteCarloSearchRobot.hpp"
#include "Robot.hpp"

class ParallelSearchRobot final : public Robot {
  static constexpr int ParallelNumber = 10;

  public:
  ParallelSearchRobot() = default;

  Span<Edge>
  BestCandidateEdges(const ScoreCountableBoard& board) override {
    if (auto edges = SubRobots.At(0).SubRobot.BestCandidateEdges(board); edges.Size() == 1) {
      return edges;
    }

    SearchResult.Reset();

#pragma omp parallel for
    for (auto& model : SubRobots) {
      model.BestCandidateEdges(board);
    }

    for (const auto& model : SubRobots) {
      SearchResult.Add(model.SearchResult);
    }

    return SearchResult.Export();
  }

  private:
  Array<MonteCarloSearchRobot, ParallelNumber> SubRobots;
  EdgeScoreMap SearchResult;
};

#pragma once

#include "MonteCarloSearchRobot.hpp"
#include "Robot.hpp"

template <int BoardSize>
class ParallelSearchRobot final : public Robot<BoardSize> {
  static constexpr int ParallelNumber = 10;

  public:
  ParallelSearchRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override {
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
  Array<MonteCarloSearchRobot<BoardSize>, ParallelNumber> SubRobots;
  EdgeScoreMap<BoardSize> SearchResult;
};

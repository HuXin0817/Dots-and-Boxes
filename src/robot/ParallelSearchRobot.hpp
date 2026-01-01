#pragma once

#include "MonteCarloSearchRobot.hpp"
#include "Robot.hpp"

template <int BoardSize>
class ParallelSearchRobot final : public Robot<BoardSize> {
#ifndef CPU_COUNT
#define CPU_COUNT 1
#endif

  static constexpr int ParallelNumber = CPU_COUNT;

  public:
  ParallelSearchRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override {
    if (auto edges = SubRobots.At(0).GetSubRobot().BestCandidateEdges(board); edges.Size() == 1) {
      return edges;
    }

    SearchResult.Reset();

#pragma omp parallel for
    for (auto& model : SubRobots) {
      model.BestCandidateEdges(board);
    }

    for (const auto& model : SubRobots) {
      SearchResult.Add(model.GetSearchResult());
    }

    return SearchResult.Export();
  }

  private:
  Array<MonteCarloSearchRobot<BoardSize>, ParallelNumber> SubRobots;
  EdgeScoreMap<BoardSize> SearchResult;
};

#pragma once

#include "MonteCarloSearchRobot.hpp"
#include "Robot.hpp"

template <int64_t BoardSize>
class ParallelSearchRobot final : public Robot<BoardSize> {
  static constexpr int ParallelNumber = CPU_COUNT;

  public:
  ParallelSearchRobot() = default;

  Span<Edge<BoardSize>, SizeType<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override {
    if (const Span<Edge<BoardSize>, SizeType<BoardSize>> edges = SubRobots.At(0).SubRobot.BestCandidateEdges(board);
        edges.Size() == 1) {
      return edges;
    }

    SearchResult.Reset();

#pragma omp parallel for
    for (MonteCarloSearchRobot<BoardSize>& model : SubRobots) {
      model.BestCandidateEdges(board);
    }

    for (const MonteCarloSearchRobot<BoardSize>& model : SubRobots) {
      SearchResult.Add(model.SearchResult);
    }

    return SearchResult.Export();
  }

  private:
  Array<MonteCarloSearchRobot<BoardSize>, ParallelNumber, int> SubRobots;
  EdgeScoreMap<BoardSize> SearchResult;
};

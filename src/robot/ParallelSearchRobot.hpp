#pragma once

#include "MonteCarloSearchRobot.hpp"
#include "Robot.hpp"

namespace dab::robot {

template <int BoardSize>
class ParallelSearchRobot final : public Robot<BoardSize> {
  static constexpr int ParallelNumber = CPU_COUNT;

  public:
  ParallelSearchRobot() = default;

  common::Span<model::Edge<BoardSize>>
  BestCandidateEdges(const board::ScoreCountableBoard<BoardSize>& board) override {
    if (const common::Span<model::Edge<BoardSize>> edges = SubRobots.At(0).SubRobot.BestCandidateEdges(board);
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
  common::Array<MonteCarloSearchRobot<BoardSize>, ParallelNumber> SubRobots;
  model::EdgeScoreMap<BoardSize> SearchResult;
};

}  // namespace dab::robot

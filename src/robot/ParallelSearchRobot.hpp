#pragma once

#include "MonteCarloSearchRobot.hpp"
#include "Robot.hpp"

namespace dab::robot {

template <int BoardSize, typename SizeType>
class ParallelSearchRobot final : public Robot<BoardSize, SizeType> {
  static constexpr int ParallelNumber = CPU_COUNT;

  public:
  ParallelSearchRobot() = default;

  common::Span<model::Edge<BoardSize, SizeType>, SizeType>
  BestCandidateEdges(const board::ScoreCountableBoard<BoardSize, SizeType>& board) override {
    if (const common::Span<model::Edge<BoardSize, SizeType>, SizeType> edges =
            SubRobots.At(0).SubRobot.BestCandidateEdges(board);
        edges.Size() == 1) {
      return edges;
    }

    SearchResult.Reset();

#pragma omp parallel for
    for (MonteCarloSearchRobot<BoardSize, SizeType>& model : SubRobots) {
      model.BestCandidateEdges(board);
    }

    for (const MonteCarloSearchRobot<BoardSize, SizeType>& model : SubRobots) {
      SearchResult.Add(model.SearchResult);
    }

    return SearchResult.Export();
  }

  private:
  common::Array<MonteCarloSearchRobot<BoardSize, SizeType>, ParallelNumber, int> SubRobots;
  model::EdgeScoreMap<BoardSize, SizeType> SearchResult;
};

}  // namespace dab::robot

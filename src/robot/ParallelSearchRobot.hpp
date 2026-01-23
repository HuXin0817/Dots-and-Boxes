#pragma once

#include "MonteCarloSearchRobot.hpp"
#include "Robot.hpp"

template <int64_t BoardSize>
class ParallelSearchRobot final : public Robot<BoardSize> {
  using SubRobotType = MonteCarloSearchRobot<BoardSize, Edge<BoardSize>::Max << 5>;

  public:
  ParallelSearchRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override {
    if (const Span<Edge<BoardSize>> edges = SubRobots.At(0).SubRobot.BestCandidateEdges(board); edges.Size() == 1) {
      return edges;
    }

    SearchResult.Reset();

#pragma omp parallel for
    for (SubRobotType& model : SubRobots) {
      model.BestCandidateEdges(board);
    }

    for (const SubRobotType& model : SubRobots) {
      SearchResult.Add(model.SearchResult);
    }

    return SearchResult.Export();
  }

  private:
  Array<SubRobotType, 64> SubRobots;
  EdgeScoreMap<BoardSize> SearchResult;
};

#pragma once

#include "MonteCarloSearchRobot.hpp"

template <int64_t BoardSize>
class ParallelSearchRobot final : public Robot<BoardSize> {
  static constexpr size_t SubRobotSearchTime = Edge<BoardSize>::Max << 5;
  static constexpr size_t SubRobotNumber = 64;
  static constexpr size_t SearchTime = SubRobotNumber * SubRobotSearchTime;

  using SubRobotType = MonteCarloSearchRobot<BoardSize, SubRobotSearchTime>;

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
  Array<SubRobotType, SubRobotNumber> SubRobots;
  EdgeScoreMap<BoardSize, Int<2 * SearchTime>> SearchResult;
};

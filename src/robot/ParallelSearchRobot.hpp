#pragma once

#include "MonteCarloSearchRobot.hpp"

namespace dab {

template <int64_t BoardSize>
class ParallelSearchRobot final : public Robot<BoardSize> {
  static constexpr int64_t SubRobotSearchTime = static_cast<int64_t>(Edge<BoardSize>::Max) << 5;
  static constexpr int64_t SubRobotNumber = 64;
  static constexpr int64_t SearchTime = SubRobotNumber * SubRobotSearchTime;

  using SubRobotType = MonteCarloSearchRobot<BoardSize, SubRobotSearchTime>;

  public:
  ParallelSearchRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override;

  private:
  Array<SubRobotType, SubRobotNumber> SubRobots;
  SearchScoreMap<BoardSize, Int<2 * SearchTime>> SearchResult;
};

template <int64_t BoardSize>
Span<Edge<BoardSize>>
ParallelSearchRobot<BoardSize>::BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) {
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

}  // namespace dab

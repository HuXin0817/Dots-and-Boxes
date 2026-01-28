#pragma once

#include "MonteCarloRobot.hpp"

namespace dab {

template <int64_t BoardSize>
class ParallelSearchRobot final : public Robot<BoardSize> {
  static constexpr int64_t SubRobotSearchTime = static_cast<int64_t>(Edge<BoardSize>::Max) << 5;
  static constexpr int64_t SubRobotNumber = 64;
  static constexpr int64_t SearchTime = SubRobotNumber * SubRobotSearchTime;

  using SubRobotType = MonteCarloRobot<BoardSize, SubRobotSearchTime>;

  public:
  ParallelSearchRobot() = default;

  Span<const Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override;

  private:
  Array<SubRobotType, SubRobotNumber> SubRobots;
  SearchScoreMap<BoardSize, Int<2 * SearchTime>> SearchResult;
};

template <int64_t BoardSize>
Span<const Edge<BoardSize>>
ParallelSearchRobot<BoardSize>::BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) {
  if (Span<const Edge<BoardSize>> edges; SubRobots.At(0).CanEarlyExit(board, edges)) {
    return edges;
  }

  SearchResult.Reset();

#pragma omp parallel for
  for (SubRobotType& model : SubRobots) {
    model.BestCandidateEdges(board);
  }

  for (const SubRobotType& model : SubRobots) {
    SearchResult.Add(model.GetSearchResult());
  }

  return SearchResult.Export();
}

}  // namespace dab

#pragma once

#include "MonteCarloRobot.hpp"

namespace dab::detail::robot {

template <int64_t BoardSize,
          int64_t SubRobotSearchTime = static_cast<int64_t>(Limits<Edge<BoardSize>>::Max) << 5,
          int64_t SubRobotNumber = 64>
class ParallelSearchRobot final : public Robot<BoardSize> {
  using SubRobotType = MonteCarloRobot<BoardSize, SubRobotSearchTime>;

  public:
  ParallelSearchRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override;

  private:
  Array<SubRobotType, SubRobotNumber> SubRobots;
  SearchScoreMap<BoardSize> SearchResult;
};

template <int64_t BoardSize, int64_t SubRobotSearchTime, int64_t SubRobotNumber>
Span<Edge<BoardSize>>
ParallelSearchRobot<BoardSize, SubRobotSearchTime, SubRobotNumber>::BestCandidateEdges(
    const ScoreCountableBoard<BoardSize>& board) {
  if (Span<Edge<BoardSize>> edges; SubRobots.Front().CanEarlyExit(board, edges)) {
    return edges;
  }

  SearchResult.Reset();

#pragma omp parallel for schedule(dynamic, 4)
  for (SubRobotType& model : SubRobots) {
    model.BestCandidateEdges(board);
  }

  for (const SubRobotType& model : SubRobots) {
    SearchResult.Add(model.GetSearchResult());
  }

  return SearchResult.Export();
}

}  // namespace dab::detail::robot
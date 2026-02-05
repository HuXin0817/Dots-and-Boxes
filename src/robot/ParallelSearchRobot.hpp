#pragma once

#include <tbb/parallel_for_each.h>

#include "MonteCarloRobot.hpp"

namespace dab::detail::robot {

template <int64_t BoardSize,
          int64_t SubRobotSearchTime = static_cast<int64_t>(Edge<BoardSize>::Max) << 5,
          int64_t SubRobotNumber = 64>
class ParallelSearchRobot final : public Robot<BoardSize> {
  public:
  ParallelSearchRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const RelativeScoreBoard<BoardSize>& board) override;

  private:
  Array<MonteCarloRobot<BoardSize, SubRobotSearchTime>, SubRobotNumber> SubRobots;
  SearchScoreMap<BoardSize> SearchResult;
};

template <int64_t BoardSize, int64_t SubRobotSearchTime, int64_t SubRobotNumber>
Span<Edge<BoardSize>>
ParallelSearchRobot<BoardSize, SubRobotSearchTime, SubRobotNumber>::BestCandidateEdges(
    const RelativeScoreBoard<BoardSize>& board) {
  if (Span<Edge<BoardSize>> edges; SubRobots.Front().CanEarlyExit(board, edges)) {
    return edges;
  }

  SearchResult.Reset();

  tbb::parallel_for_each(SubRobots, [&](MonteCarloRobot<BoardSize, SubRobotSearchTime>& robot) -> void {
    robot.BestCandidateEdges(board);
  });

  for (const MonteCarloRobot<BoardSize, SubRobotSearchTime>& model : SubRobots) {
    SearchResult.Add(model.GetSearchResult());
  }

  return SearchResult.Export();
}

}  // namespace dab::detail::robot
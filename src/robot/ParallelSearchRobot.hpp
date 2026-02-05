#pragma once

#include <tbb/parallel_for_each.h>

#include "MonteCarloRobot.hpp"

namespace dab::detail::robot {

template <int64_t BoardSize>
class ParallelSearchRobot final : public Robot<BoardSize> {
  static constexpr int64_t SubRobotNumber = 32;

  public:
  ParallelSearchRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const RelativeScoreBoard<BoardSize>& board) override;

  private:
  Array<MonteCarloRobot<BoardSize>, SubRobotNumber> SubRobots;
  SearchScoreMap<BoardSize> SearchResult;
};

template <int64_t BoardSize>
Span<Edge<BoardSize>>
ParallelSearchRobot<BoardSize>::BestCandidateEdges(const RelativeScoreBoard<BoardSize>& board) {
  if (Span<Edge<BoardSize>> edges; SubRobots.Front().CanEarlyExit(board, edges)) {
    return edges;
  }

  SearchResult.Reset();

  tbb::parallel_for_each(SubRobots,
                         [&](MonteCarloRobot<BoardSize>& robot) -> void { robot.BestCandidateEdges(board); });

  for (const MonteCarloRobot<BoardSize>& model : SubRobots) {
    SearchResult.Add(model.GetSearchResult());
  }

  return SearchResult.Export();
}

}  // namespace dab::detail::robot
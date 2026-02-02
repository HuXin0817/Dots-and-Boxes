#pragma once

#include <tbb/parallel_for.h>

#include "MonteCarloRobot.hpp"

namespace dab::detail::robot {

template <int64_t BoardSize,
          int64_t SubRobotSearchTime = static_cast<int64_t>(Edge<BoardSize>::Max) << 5,
          int64_t SubRobotNumber = 64>
class ParallelSearchRobot final : public Robot<BoardSize> {
  public:
  ParallelSearchRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override;

  private:
  Array<MonteCarloRobot<BoardSize, SubRobotSearchTime>, SubRobotNumber> SubRobots;
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

  tbb::parallel_for(tbb::blocked_range<size_t>(0, SubRobots.Size()), [&](const tbb::blocked_range<size_t>& r) {
    for (size_t i = r.begin(); i != r.end(); ++i) {
      SubRobots[i].BestCandidateEdges(board);
    }
  });

  for (const MonteCarloRobot<BoardSize, SubRobotSearchTime>& model : SubRobots) {
    SearchResult.Add(model.GetSearchResult());
  }

  return SearchResult.Export();
}

}  // namespace dab::detail::robot
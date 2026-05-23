#pragma once

#include <tbb/parallel_for_each.h>

#include "MonteCarloRobot.h"

namespace dab::__detail__::robot {

class ParallelSearchRobot {
  static constexpr std::int64_t WorkersNumber = std::min<std::int64_t>(NUM_CPUS, Edge::Max);

 public:
  ParallelSearchRobot() = default;
  template <typename Board>
  Span<const Edge> BestCandidateEdges(const Board& board);

 private:
  Array<MonteCarloRobot, WorkersNumber> Workers;
};

template <typename Board>
Span<const Edge> ParallelSearchRobot::BestCandidateEdges(const Board& board) {
  MonteCarloRobot& front = Workers.Front();
  if (Span<const Edge> edges; front.CanEarlyExit(board, edges)) {
    return edges;
  }

  tbb::parallel_for_each(Workers, [&](MonteCarloRobot& robot) -> void { robot.SearchCandidateEdges(board); });
  for (Int i = 1; i < Workers.Size(); i++) {
    front.GetSearchResult().Add(Workers.At(i).GetSearchResult());
  }

  return front.GetSearchResult().Export(front.GetSearchEdges());
}

}  // namespace dab::__detail__::robot
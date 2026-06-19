#pragma once

#include <tbb/parallel_for_each.h>

#include "MonteCarloRobot.h"

namespace dab::__detail__::robot {

class ParallelSearchRobot {
 public:
  static inline std::int64_t WorkersNumber =
      std::clamp<std::int64_t>(std::thread::hardware_concurrency() - 1, 1, model::Edge::Max);
  static inline std::int64_t SearchTime = WorkersNumber * MonteCarloRobot::SearchTime;

  ParallelSearchRobot() { Workers.Reset(WorkersNumber); }
  template <typename Board>
  iterable::Span<const model::Edge> BestCandidateEdges(const Board& board);

 private:
  iterable::Vector<MonteCarloRobot> Workers;
};

}  // namespace dab::__detail__::robot

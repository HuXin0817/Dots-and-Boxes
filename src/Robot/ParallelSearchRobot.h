#pragma once

#include <tbb/parallel_for_each.h>

#include "MonteCarloRobot.h"

namespace dab::__detail__::robot {

class ParallelSearchRobot {
 public:
  static inline std::int64_t WorkersNumber =
      std::clamp<std::int64_t>(std::thread::hardware_concurrency() - 2, 1, Edge::Max);
  static inline std::int64_t SearchTime = WorkersNumber * MonteCarloRobot::SearchTime;

  ParallelSearchRobot() { Workers.Reset(WorkersNumber); }
  template <typename Board>
  Span<const Edge> BestCandidateEdges(const Board& board);

 private:
  Vector<MonteCarloRobot> Workers;
};

template <typename Board>
Span<const Edge> ParallelSearchRobot::BestCandidateEdges(const Board& board) {
  MonteCarloRobot& front = Workers.Front();
  if (Span<const Edge> edges; front.CanEarlyExit(board, edges)) {
    return edges;
  }

  std::atomic<std::int64_t> remain = SearchTime / board.RemainStep();

  tbb::task_arena arena(WorkersNumber);
  arena.execute([&]() -> void {
    tbb::parallel_for_each(Workers, [&](MonteCarloRobot& worker) -> void {
      worker.GetSearchResult().Reset();
      while (remain > 0) {
        remain--;
        worker.SearchOnce(board);
      }
    });
  });

  for (Int i = 1; i < Workers.Size(); i++) {
    front.GetSearchResult().Add(Workers.At(i).GetSearchResult());
  }

  return front.GetSearchResult().Export(front.GetSearchEdges());
}

}  // namespace dab::__detail__::robot
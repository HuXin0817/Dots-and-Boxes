#include "ParallelSearchRobot.h"

namespace dab::__detail__::robot {

template <typename Board>
iterable::Span<const model::Edge> ParallelSearchRobot::BestCandidateEdges(const Board& board) {
  MonteCarloRobot& front = Workers.Front();
  if (iterable::Span<const model::Edge> edges; front.CanEarlyExit(board, edges)) {
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

template iterable::Span<const model::Edge> ParallelSearchRobot::BestCandidateEdges<board::GameBoard>(
    const board::GameBoard& board);

}  // namespace dab::__detail__::robot
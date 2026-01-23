#pragma once

#include "MonteCarloSearchRobot.hpp"
#include "Robot.hpp"

template <int64_t BoardSize>
class ParallelSearchRobot final : public Robot<BoardSize> {
  using SubRobotType = MonteCarloSearchRobot<BoardSize, Edge<BoardSize>::Max << 5>;

  public:
  ParallelSearchRobot() = default;

  Span<Edge<BoardSize>, SizeType<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override {
    if (const Span<Edge<BoardSize>, SizeType<BoardSize>> edges =
            ImprovedSearchRobot<BoardSize>().BestCandidateEdges(board);
        edges.Size() == 1) {
      return edges;
    }

    SearchResult.Reset();

#pragma omp parallel for
    for (const int i : std::views::iota(0, 64)) {
      SubRobotType().SearchCandidateEdges(board, SearchResult);
    }

    return SearchResult.Export();
  }

  private:
  EdgeScoreMap<BoardSize, std::atomic<int>> SearchResult;
};

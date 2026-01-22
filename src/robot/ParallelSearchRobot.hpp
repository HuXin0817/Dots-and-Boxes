#pragma once

#include "MonteCarloSearchRobot.hpp"
#include "Robot.hpp"

template <int BoardSize, typename SizeType>
class ParallelSearchRobot final : public Robot<BoardSize, SizeType> {
  static constexpr int ParallelNumber = CPU_COUNT;

  public:
  ParallelSearchRobot() = default;

  Span<Edge<BoardSize, SizeType>, SizeType>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize, SizeType>& board) override {
    if (const Span<Edge<BoardSize, SizeType>, SizeType> edges = SubRobots.At(0).SubRobot.BestCandidateEdges(board);
        edges.Size() == 1) {
      return edges;
    }

    SearchResult.Reset();

#pragma omp parallel for
    for (MonteCarloSearchRobot<BoardSize, SizeType>& model : SubRobots) {
      model.BestCandidateEdges(board);
    }

    for (const MonteCarloSearchRobot<BoardSize, SizeType>& model : SubRobots) {
      SearchResult.Add(model.SearchResult);
    }

    return SearchResult.Export();
  }

  private:
  Array<MonteCarloSearchRobot<BoardSize, SizeType>, ParallelNumber, int> SubRobots;
  EdgeScoreMap<BoardSize, SizeType> SearchResult;
};

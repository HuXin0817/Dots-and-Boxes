#pragma once

#include "MonteCarloSearchModel.hpp"
#include "SearchModel.hpp"

class ParallelSearchModel final : public SearchModel {
  public:
  static constexpr int ParallelNumber = 10;

  ParallelSearchModel() = default;

  Span<Edge>
  BestCandidateEdges(const BoardV2& board) override {
    if (auto edges = SubModels.At(0).SubModel.BestCandidateEdges(board); edges.Size() == 1) {
      return edges;
    }

    result.Reset();

#pragma omp parallel for
    for (auto& model : SubModels) {
      model.BestCandidateEdges(board);
    }

    for (const auto& model : SubModels) {
      result.Add(model.ScoreMap);
    }

    return result.Export();
  }

  private:
  Array<MonteCarloSearchModel, ParallelNumber> SubModels;
  EdgeScoreMap result;
};

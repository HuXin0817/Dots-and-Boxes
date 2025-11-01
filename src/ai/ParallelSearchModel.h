#pragma once

#include "MonteCarloSearchModel.h"
#include "SearchModel.h"

class ParallelSearchModel final : public SearchModel {
  public:
  static constexpr int SearchGroupNumber = 100;

  ParallelSearchModel() = default;

  Span<Edge>
  BestCandidateEdges(const BoardV2& board) override {
    if (auto edges = ImprovedSearchModel().BestCandidateEdges(board); edges.Size() == 1) {
      return edges;
    }

    thread_local MonteCarloSearchModel model;
    EdgeScoreMap result;

#pragma omp parallel for
    for (int i = 0; i < SearchGroupNumber; ++i) {
      model.BestCandidateEdges(board);
#pragma omp critical
      {
        result.Add(model.ScoreMap);
      }
    }

    return result.Export();
  }
};

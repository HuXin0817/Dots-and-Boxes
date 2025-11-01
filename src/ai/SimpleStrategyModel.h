#pragma once

#include "../board/BoardV2.h"
#include "../common/List.h"
#include "../common/Span.h"
#include "SearchModel.h"

class SimpleStrategyModel final : public SearchModel {
  friend class BasicSearchModel;
  friend class ImprovedSearchModel;

  public:
  SimpleStrategyModel() = default;

  Span<Edge>
  BestCandidateEdges(const BoardV2& board) override {
    EnemyUnscoreableEdges.Clear();
    ScoreableEdges.Clear();
    auto EmptyEdges = board.EmptyEdges();

    for (Edge edge : EmptyEdges) {
      if (int maxCount = board.MaxCount(edge); maxCount == 3) {
        ScoreableEdges.Append(edge);
      } else if (maxCount < 2) {
        EnemyUnscoreableEdges.Append(edge);
      }
    }

    if (!ScoreableEdges.Empty()) {
      return ScoreableEdges.Export();
    }
    if (!EnemyUnscoreableEdges.Empty()) {
      return EnemyUnscoreableEdges.Export();
    }

    return {EmptyEdges.begin(), EmptyEdges.end()};
  }

  private:
  List<Edge, Edge::Max> EnemyUnscoreableEdges;
  List<Edge, Edge::Max> ScoreableEdges;
};

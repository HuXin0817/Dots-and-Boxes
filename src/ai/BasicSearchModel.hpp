#pragma once

#include "../board/BoardV3.hpp"
#include "../common/Span.hpp"
#include "SearchModel.hpp"
#include "SimpleStrategyModel.hpp"

class BasicSearchModel final : public SearchModel {
  friend class ImprovedSearchModel;

  public:
  Span<Edge>
  BestCandidateEdges(const BoardV2& board) override {
    if (auto edges = SubModel.BestCandidateEdges(board);
        !SubModel.EnemyUnscoreableEdges.Empty() || !SubModel.ScoreableEdges.Empty()) {
      return edges;
    }

    int minScore = Box::Max + 1;
    auto& candidateEdges = SubModel.EnemyUnscoreableEdges;
    assert(candidateEdges.Empty());

    for (Edge edge : board.EmptyEdges()) {
      AuxBoard.Reset(board.GetBoardV1());
#ifdef NDEBUG
      AuxBoard.Add(edge);
#else
      assert(AuxBoard.Add(edge) == 0);
#endif
      if (int score = AuxBoard.MaxObtainableScore(minScore); score < minScore) {
        minScore = score;
        candidateEdges.Reset(edge);
      } else if (score == minScore) {
        candidateEdges.Append(edge);
      }
    }

    return Export(candidateEdges);
  }

  private:
  SimpleStrategyModel SubModel;
  BoardV3 AuxBoard;
};

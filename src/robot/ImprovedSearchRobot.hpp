#pragma once

#include "../common/List.hpp"
#include "../common/Span.hpp"
#include "BasicSearchRobot.hpp"

class ImprovedSearchRobot final : public Robot {
  public:
  ImprovedSearchRobot() = default;

  Span<Edge>
  BestCandidateEdges(const BoardV2& board) override {
    if (auto edges = SubModel.BestCandidateEdges(board);
        !SubModel.SubModel.EnemyUnscoreableEdges.Empty()) {
      return edges;
    }

    SearchEdges.Clear();
    int maxScore = -Box::Max;
    for (Edge emptyEdge : board.EmptyEdges()) {
      AuxBoard.Reset(board.GetBoardV1());
      AuxBoard.Add(emptyEdge);
      while (AuxBoard.Gaming()) {
        Edge edge = SubModel.BestCandidateEdges(AuxBoard).At(0);
        assert(board.MaxCount(edge) > 1);
        AuxBoard.Add(edge);
      }
      if (int score = AuxBoard.Score(); score > maxScore) {
        maxScore = score;
        SearchEdges.Reset(emptyEdge);
      } else if (score == maxScore) {
        SearchEdges.Append(emptyEdge);
      }
    }

    return Export(SearchEdges);
  }

  private:
  BasicSearchRobot SubModel;
  BoardV2 AuxBoard;
  List<Edge, Edge::Max> SearchEdges;
};

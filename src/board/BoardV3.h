#pragma once

#include "../common/Queue.h"
#include "../model/Edge.h"
#include "BoardV1.h"

class BoardV3 : public BoardV1 {
  public:
  BoardV3() = default;

  void
  Reset(const BoardV1& newBoard) {
    GetBoardV1() = newBoard;
    ScoreableEdges.Clear();
  }

  int
  Add(Edge edge) {
    int score = BoardV1::Add(edge);
    for (Box box : EdgeBoxMapper::EdgeNearBoxes.At(edge)) {
      if (EdgeCountOfBox::At(box) == 3) {
        Edge edgeToAdd = FindNotContainsEdgeInBox(box);
        ScoreableEdges.Append(edgeToAdd);
      }
    }
    return score;
  }

  int
  MaxObtainableScore(int minScore) {
    int score = 0;
    while (Gaming()) {
      if (ScoreableEdges.Empty()) {
        if (Edge edge = FindScoreableEdge(); edge != -1) {
          ScoreableEdges.Append(edge);
        } else {
          break;
        }
      }
      Edge edge = ScoreableEdges.Pop();
      if (Contains(edge)) {
        continue;
      }
      int addScore = Add(edge);
      assert(addScore > 0);
      score += addScore;
      if (score >= minScore) {
        break;
      }
    }
    return score;
  }

  private:
  Queue<Edge, Edge::Max> ScoreableEdges;
};

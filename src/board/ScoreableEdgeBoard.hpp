#pragma once

#include "../common/Queue.hpp"
#include "../model/Edge.hpp"
#include "EdgeCountableBoard.hpp"

template <int BoardSize>
class ScoreableEdgeBoard : public EdgeCountableBoard<BoardSize> {
  public:
  ScoreableEdgeBoard() = default;

  void
  Reset(const EdgeCountableBoard<BoardSize>& newBoard) {
    EdgeCountableBoard<BoardSize>::GetEdgeCountableBoard() = newBoard;
    ScoreableEdges.Clear();
  }

  int
  Add(Edge<BoardSize> edge) {
    int score = EdgeCountableBoard<BoardSize>::Add(edge);
    for (Box box : NearBoxes(edge)) {
      if (EdgeCountOfBox<BoardSize>::At(box) == 3) {
        Edge edgeToAdd = EdgeCountableBoard<BoardSize>::FindNotContainsEdgeInBox(box);
        ScoreableEdges.Append(edgeToAdd);
      }
    }
    return score;
  }

  int
  MaxObtainableScore(int minScore) {
    int score = 0;
    while (EdgeCountableBoard<BoardSize>::Gaming()) {
      if (ScoreableEdges.Empty()) {
        if (Edge edge = EdgeCountableBoard<BoardSize>::FindScoreableEdge(); edge != -1) {
          ScoreableEdges.Append(edge);
        } else {
          break;
        }
      }
      Edge edge = ScoreableEdges.Pop();
      if (EdgeCountableBoard<BoardSize>::Contains(edge)) {
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
  Queue<Edge<BoardSize>, Edge<BoardSize>::Max> ScoreableEdges;
};

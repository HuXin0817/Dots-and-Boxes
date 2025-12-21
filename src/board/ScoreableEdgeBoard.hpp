#pragma once

#include "../common/Queue.hpp"
#include "../model/Edge.hpp"
#include "EdgeCountableBoard.hpp"

template <int BoardSize>
class ScoreableEdgeBoard {
  public:
  ScoreableEdgeBoard() = default;

  void
  Reset(const EdgeCountableBoard<BoardSize>& newBoard) {
    EdgeCountableBoard.GetEdgeCountableBoard() = newBoard;
    ScoreableEdges.Clear();
  }

  int
  Add(Edge<BoardSize> edge) {
    int score = EdgeCountableBoard.Add(edge);
    for (Box box : NearBoxes(edge)) {
      if (EdgeCountableBoard.GetEdgeCountOfBox().At(box) == 3) {
        Edge edgeToAdd = EdgeCountableBoard.FindNotContainsEdgeInBox(box);
        ScoreableEdges.Append(edgeToAdd);
      }
    }
    return score;
  }

  int
  MaxObtainableScore(int minScore) {
    int score = 0;
    while (EdgeCountableBoard.GetBasicBoard().GetStep().Gaming()) {
      if (ScoreableEdges.Empty()) {
        if (Edge edge = EdgeCountableBoard.FindScoreableEdge(); edge != -1) {
          ScoreableEdges.Append(edge);
        } else {
          break;
        }
      }
      Edge edge = ScoreableEdges.Pop();
      if (EdgeCountableBoard.GetBasicBoard().Contains(edge)) {
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
  EdgeCountableBoard<BoardSize> EdgeCountableBoard;
  Queue<Edge<BoardSize>, Edge<BoardSize>::Max> ScoreableEdges;
};

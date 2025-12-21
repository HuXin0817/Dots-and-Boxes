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
    EdgeCountableBoard = newBoard;
    ScoreableEdges.Clear();
  }

  int
  Add(Edge<BoardSize> edge) {
    int score = EdgeCountableBoard.Add(edge);
    for (Box<BoardSize> box : NearBoxes(edge)) {
      if (EdgeCountableBoard.GetEdgeCountOfBox().At(box.Int()) == 3) {
        Edge<BoardSize> edgeToAdd = EdgeCountableBoard.FindNotContainsEdgeInBox(box);
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
        if (Edge<BoardSize> edge = EdgeCountableBoard.FindScoreableEdge(); edge.Int() != -1) {
          ScoreableEdges.Append(edge);
        } else {
          break;
        }
      }
      Edge<BoardSize> edge = ScoreableEdges.Pop();
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

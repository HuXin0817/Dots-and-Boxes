#pragma once

#include "../common/Queue.hpp"
#include "../model/Edge.hpp"
#include "EdgeCountableBoard.hpp"

template <int64_t BoardSize>
class ScoreableEdgeBoard {
  public:
  ScoreableEdgeBoard() = default;

  void
  Reset(const EdgeCountableBoard<BoardSize>& newBoard) {
    EdgeCountableBoard = newBoard;
    ScoreableEdges.Clear();
  }

  SizeType
  Add(const Edge<BoardSize> edge) {
    const SizeType score = EdgeCountableBoard.Add(edge);
    for (const Box<BoardSize> box : NearBoxes(edge)) {
      if (EdgeCountableBoard.GetEdgeCountOfBox().EdgeCount(box) == 3) {
        ScoreableEdges.Append(EdgeCountableBoard.FindNotContainsEdgeInBox(box));
      }
    }
    return score;
  }

  SizeType
  MaxObtainableScore(const SizeType minScore) {
    SizeType score = 0;
    while (EdgeCountableBoard.GetBasicBoard().GetStep().Gaming()) {
      if (ScoreableEdges.Empty()) {
        if (const Edge<BoardSize> edge = EdgeCountableBoard.FindScoreableEdge();
            edge.Value() != InvalidEdge<BoardSize>().Value()) {
          ScoreableEdges.Append(edge);
        } else {
          break;
        }
      }
      const Edge<BoardSize> edge = ScoreableEdges.Pop();
      if (EdgeCountableBoard.GetBasicBoard().Contains(edge)) {
        continue;
      }
      const SizeType addScore = Add(edge);
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

#pragma once

#include "../common/Queue.hpp"
#include "../model/Edge.hpp"
#include "EdgeCountableBoard.hpp"

namespace dab::board {

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
  Add(const model::Edge<BoardSize> edge) {
    const int score = EdgeCountableBoard.Add(edge);
    for (const model::Box<BoardSize> box : NearBoxes(edge)) {
      if (EdgeCountableBoard.GetEdgeCountOfBox().EdgeCount(box) == 3) {
        ScoreableEdges.Append(EdgeCountableBoard.FindNotContainsEdgeInBox(box));
      }
    }
    return score;
  }

  int
  MaxObtainableScore(const int minScore) {
    int score = 0;
    while (EdgeCountableBoard.GetBasicBoard().GetStep().Gaming()) {
      if (ScoreableEdges.Empty()) {
        if (const model::Edge<BoardSize> edge = EdgeCountableBoard.FindScoreableEdge();
            edge.Value() != model::InvalidEdge<BoardSize>().Value()) {
          ScoreableEdges.Append(edge);
        } else {
          break;
        }
      }
      const model::Edge<BoardSize> edge = ScoreableEdges.Pop();
      if (EdgeCountableBoard.GetBasicBoard().Contains(edge)) {
        continue;
      }
      const int addScore = Add(edge);
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
  common::Queue<model::Edge<BoardSize>, model::Edge<BoardSize>::Max, int> ScoreableEdges;
};

}  // namespace dab::board

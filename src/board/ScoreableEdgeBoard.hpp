#pragma once

#include "../common/Queue.hpp"
#include "../model/Edge.hpp"
#include "EdgeCountableBoard.hpp"

namespace dab::board {

template <int BoardSize, typename SizeType>
class ScoreableEdgeBoard {
  public:
  ScoreableEdgeBoard() = default;

  void
  Reset(const EdgeCountableBoard<BoardSize, SizeType>& newBoard) {
    EdgeCountableBoard = newBoard;
    ScoreableEdges.Clear();
  }

  SizeType
  Add(const model::Edge<BoardSize, SizeType> edge) {
    const SizeType score = EdgeCountableBoard.Add(edge);
    for (const model::Box<BoardSize, SizeType> box : NearBoxes(edge)) {
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
        if (const model::Edge<BoardSize, SizeType> edge = EdgeCountableBoard.FindScoreableEdge();
            edge.Value() != model::InvalidEdge<BoardSize, SizeType>().Value()) {
          ScoreableEdges.Append(edge);
        } else {
          break;
        }
      }
      const model::Edge<BoardSize, SizeType> edge = ScoreableEdges.Pop();
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
  EdgeCountableBoard<BoardSize, SizeType> EdgeCountableBoard;
  common::Queue<model::Edge<BoardSize, SizeType>, model::Edge<BoardSize, SizeType>::Max, SizeType> ScoreableEdges;
};

}  // namespace dab::board

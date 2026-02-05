#pragma once

#include <Dab/Model.hpp>

#include "EdgeCountableBoard.hpp"

namespace dab::detail::board {

template <int64_t BoardSize>
class ScoreableEdgeBoard : public EdgeCountableBoard<BoardSize> {
  public:
  ScoreableEdgeBoard();

  void Reset(const EdgeCountableBoard<BoardSize>& newBoard = EdgeCountableBoard<BoardSize>());
  Int<BoardSize> Add(const Edge<BoardSize> edge);
  Int<BoardSize> MaxObtainableScore(const Int<BoardSize> minScore);

  private:
  Queue<Edge<BoardSize>, Edge<BoardSize>::Max> ScoreableEdges;
};

template <int64_t BoardSize>
ScoreableEdgeBoard<BoardSize>::ScoreableEdgeBoard() {
  Reset();
}

template <int64_t BoardSize>
void ScoreableEdgeBoard<BoardSize>::Reset(const EdgeCountableBoard<BoardSize>& newBoard) {
  EdgeCountableBoard<BoardSize>::operator=(newBoard);
  ScoreableEdges.Clear();
}

template <int64_t BoardSize>
Int<BoardSize> ScoreableEdgeBoard<BoardSize>::Add(const Edge<BoardSize> edge) {
  const Int<BoardSize> score = EdgeCountableBoard<BoardSize>::Add(edge);
  for (const Box<BoardSize> box : NearBoxes(edge)) {
    if (EdgeCountableBoard<BoardSize>::EdgeCount(box) == 3) {
      ScoreableEdges.Append(EdgeCountableBoard<BoardSize>::FindNotContainsEdgeInBox(box));
    }
  }
  return score;
}

template <int64_t BoardSize>
Int<BoardSize> ScoreableEdgeBoard<BoardSize>::MaxObtainableScore(const Int<BoardSize> minScore) {
  Int<BoardSize> score = 0;
  while (EdgeCountableBoard<BoardSize>::Gaming()) {
    if (ScoreableEdges.Empty()) {
      if (const Edge<BoardSize> edge = EdgeCountableBoard<BoardSize>::FindScoreableEdge();
          edge != InvalidEdge<BoardSize>) {
        ScoreableEdges.Append(edge);
      } else {
        break;
      }
    }
    const Edge<BoardSize> edge = ScoreableEdges.Pop();
    if (EdgeCountableBoard<BoardSize>::Contains(edge)) {
      continue;
    }
    const Int<BoardSize> addScore = Add(edge);
    assert(addScore > 0);
    score += addScore;
    if (score >= minScore) {
      break;
    }
  }
  return score;
}

}  // namespace dab::detail::board
#pragma once

#include "../common/Queue.hpp"
#include "../model/Edge.hpp"
#include "EdgeCountableBoard.hpp"

namespace dab {

template <int64_t BoardSize>
class ScoreableEdgeBoard : public EdgeCountableBoard<BoardSize> {
  public:
  ScoreableEdgeBoard();

  void
  Reset(const EdgeCountableBoard<BoardSize>& newBoard = EdgeCountableBoard<BoardSize>());
  SizeType<BoardSize>
  Add(const Edge<BoardSize> edge);
  SizeType<BoardSize>
  MaxObtainableScore(const SizeType<BoardSize> minScore);

  private:
  Queue<Edge<BoardSize>, Limits<Edge<BoardSize>>::Max> ScoreableEdges;
};

template <int64_t BoardSize>
ScoreableEdgeBoard<BoardSize>::ScoreableEdgeBoard() {
  Reset();
}

template <int64_t BoardSize>
void
ScoreableEdgeBoard<BoardSize>::Reset(const EdgeCountableBoard<BoardSize>& newBoard) {
  EdgeCountableBoard<BoardSize>::operator=(newBoard);
  ScoreableEdges.Clear();
}

template <int64_t BoardSize>
SizeType<BoardSize>
ScoreableEdgeBoard<BoardSize>::Add(const Edge<BoardSize> edge) {
  const SizeType<BoardSize> score = EdgeCountableBoard<BoardSize>::Add(edge);
  for (const Box<BoardSize> box : NearBoxes(edge)) {
    if (EdgeCountableBoard<BoardSize>::EdgeCount(box) == 3) {
      ScoreableEdges.Append(EdgeCountableBoard<BoardSize>::FindNotContainsEdgeInBox(box));
    }
  }
  return score;
}

template <int64_t BoardSize>
SizeType<BoardSize>
ScoreableEdgeBoard<BoardSize>::MaxObtainableScore(const SizeType<BoardSize> minScore) {
  SizeType<BoardSize> score = 0;
  while (EdgeCountableBoard<BoardSize>::Gaming()) {
    if (ScoreableEdges.Empty()) {
      if (const Edge<BoardSize> edge = EdgeCountableBoard<BoardSize>::FindScoreableEdge();
          edge != InvalidEdge<BoardSize>()) {
        ScoreableEdges.Append(edge);
      } else {
        break;
      }
    }
    const Edge<BoardSize> edge = ScoreableEdges.Pop();
    if (EdgeCountableBoard<BoardSize>::Contains(edge)) {
      continue;
    }
    const SizeType<BoardSize> addScore = Add(edge);
    assert(addScore > 0);
    score += addScore;
    if (score >= minScore) {
      break;
    }
  }
  return score;
}

}  // namespace dab

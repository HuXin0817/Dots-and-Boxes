#pragma once

#include "../model/Edge.hpp"
#include "../model/EdgeCounter.hpp"
#include "../model/NearEdges.hpp"
#include "../model/Square.hpp"
#include "BasicBoard.hpp"

namespace dab {

template <int64_t BoardSize>
class EdgeCountableBoard : public BasicBoard<BoardSize>, public EdgeCounter<BoardSize> {
  public:
  EdgeCountableBoard();

  void
  Reset();
  SizeType<BoardSize>
  Add(const Edge<BoardSize> edge);
  Edge<BoardSize>
  FindNotContainsEdgeInBox(const Box<BoardSize> box) const;
  Edge<BoardSize>
  FindScoreableEdge() const;
};

template <int64_t BoardSize>
EdgeCountableBoard<BoardSize>::EdgeCountableBoard() {
  Reset();
}

template <int64_t BoardSize>
void
EdgeCountableBoard<BoardSize>::Reset() {
  BasicBoard<BoardSize>::Reset();
  EdgeCounter<BoardSize>::Reset();
}

template <int64_t BoardSize>
SizeType<BoardSize>
EdgeCountableBoard<BoardSize>::Add(const Edge<BoardSize> edge) {
  BasicBoard<BoardSize>::Add(edge);
  return EdgeCounter<BoardSize>::Add(edge);
}

template <int64_t BoardSize>
Edge<BoardSize>
EdgeCountableBoard<BoardSize>::FindNotContainsEdgeInBox(const Box<BoardSize> box) const {
  assert(EdgeCounter<BoardSize>::EdgeCount(box) == 3);
  for (const Edge<BoardSize> edge : NearEdges(box)) {
    if (!BasicBoard<BoardSize>::Contains(edge)) {
      return edge;
    }
  }
  assert(false);
  return InvalidEdge<BoardSize>();
}

template <int64_t BoardSize>
Edge<BoardSize>
EdgeCountableBoard<BoardSize>::FindScoreableEdge() const {
  for (const Box<BoardSize> box : Iota<Box<BoardSize>>()) {
    if (EdgeCounter<BoardSize>::EdgeCount(box) == 3) {
      return FindNotContainsEdgeInBox(box);
    }
  }
  return InvalidEdge<BoardSize>();
}

}  // namespace dab

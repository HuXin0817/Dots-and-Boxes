#pragma once

#include "HashBoard.hpp"
#include "Model.hpp"

namespace dab::detail::board {

template <int64_t BoardSize>
class EdgeCountableBoard : public HashBoard<BoardSize>, public EdgeCounter<BoardSize> {
  public:
  EdgeCountableBoard();

  void
  Reset();
  Int<BoardSize>
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
  HashBoard<BoardSize>::Reset();
  EdgeCounter<BoardSize>::Reset();
}

template <int64_t BoardSize>
Int<BoardSize>
EdgeCountableBoard<BoardSize>::Add(const Edge<BoardSize> edge) {
  HashBoard<BoardSize>::Add(edge);
  return EdgeCounter<BoardSize>::Add(edge);
}

template <int64_t BoardSize>
Edge<BoardSize>
EdgeCountableBoard<BoardSize>::FindNotContainsEdgeInBox(const Box<BoardSize> box) const {
  assert(EdgeCounter<BoardSize>::EdgeCount(box) == 3);
  for (const Edge<BoardSize> edge : NearEdges(box)) {
    if (!HashBoard<BoardSize>::Contains(edge)) {
      return edge;
    }
  }
  assert(false);
  return InvalidEdge<BoardSize>;
}

template <int64_t BoardSize>
Edge<BoardSize>
EdgeCountableBoard<BoardSize>::FindScoreableEdge() const {
  for (Box<BoardSize> box = 0; box < Box<BoardSize>::Max; ++box) {
    if (EdgeCounter<BoardSize>::EdgeCount(box) == 3) {
      return FindNotContainsEdgeInBox(box);
    }
  }
  return InvalidEdge<BoardSize>;
}

}  // namespace dab::detail::board
#pragma once

#include <Dab/Model.hpp>

#include "HashBoard.hpp"

namespace dab::detail::board {

template <int64_t BoardSize>
class EdgeCountableBoard : public HashBoard<BoardSize> {
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
  uint8_t
  EdgeCount(const Box<BoardSize> box) const;
  uint8_t
  MaxEdgeCount(const Edge<BoardSize> edge) const;

  private:
  Array<uint8_t, Box<BoardSize>::Max> Counter;
};

template <int64_t BoardSize>
EdgeCountableBoard<BoardSize>::EdgeCountableBoard() {
  Reset();
}

template <int64_t BoardSize>
void
EdgeCountableBoard<BoardSize>::Reset() {
  HashBoard<BoardSize>::Reset();
  Counter = Array<uint8_t, Box<BoardSize>::Max>();
}

template <int64_t BoardSize>
Int<BoardSize>
EdgeCountableBoard<BoardSize>::Add(const Edge<BoardSize> edge) {
  HashBoard<BoardSize>::Add(edge);
  Int<BoardSize> score = 0;
  for (const Box<BoardSize> box : NearBoxes(edge)) {
    ++Counter[box];
    const uint8_t num = Counter[box];
    assert(num <= 4);
    if (num == 4) {
      ++score;
    }
  }
  return score;
}

template <int64_t BoardSize>
Edge<BoardSize>
EdgeCountableBoard<BoardSize>::FindNotContainsEdgeInBox(const Box<BoardSize> box) const {
  assert(EdgeCount(box) == 3);
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
    if (EdgeCount(box) == 3) {
      return FindNotContainsEdgeInBox(box);
    }
  }
  return InvalidEdge<BoardSize>;
}

template <int64_t BoardSize>
uint8_t
EdgeCountableBoard<BoardSize>::EdgeCount(const Box<BoardSize> box) const {
  return Counter[box];
}

template <int64_t BoardSize>
uint8_t
EdgeCountableBoard<BoardSize>::MaxEdgeCount(const Edge<BoardSize> edge) const {
  uint8_t maxCount = 0;
  for (const Box<BoardSize> box : NearBoxes(edge)) {
    maxCount = std::max(maxCount, EdgeCount(box));
  }
  return maxCount;
}

}  // namespace dab::detail::board
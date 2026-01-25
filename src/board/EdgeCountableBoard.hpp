#pragma once

#include "../model/Edge.hpp"
#include "../model/EdgeCountOfBox.hpp"
#include "../model/NearEdges.hpp"
#include "../model/Square.hpp"
#include "BasicBoard.hpp"

template <int64_t BoardSize>
class EdgeCountableBoard : public BasicBoard<BoardSize>, public EdgeCountOfBox<BoardSize> {
  public:
  EdgeCountableBoard() = default;

  SizeType<BoardSize>
  Add(const Edge<BoardSize> edge);
  Edge<BoardSize>
  FindNotContainsEdgeInBox(const Box<BoardSize> box) const;
  Edge<BoardSize>
  FindScoreableEdge() const;
  const EdgeCountableBoard&
  GetEdgeCountableBoard() const;
};

template <int64_t BoardSize>
SizeType<BoardSize>
EdgeCountableBoard<BoardSize>::Add(const Edge<BoardSize> edge) {
  BasicBoard<BoardSize>::Add(edge);
  return EdgeCountOfBox<BoardSize>::Add(edge);
}

template <int64_t BoardSize>
Edge<BoardSize>
EdgeCountableBoard<BoardSize>::FindNotContainsEdgeInBox(const Box<BoardSize> box) const {
  assert(EdgeCountOfBox<BoardSize>::EdgeCount(box) == 3);
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
  for (const Box<BoardSize> box : Iota(Box<BoardSize>::Max)) {
    if (EdgeCountOfBox<BoardSize>::EdgeCount(box) == 3) {
      return FindNotContainsEdgeInBox(box);
    }
  }
  return InvalidEdge<BoardSize>();
}

template <int64_t BoardSize>
const EdgeCountableBoard<BoardSize>&
EdgeCountableBoard<BoardSize>::GetEdgeCountableBoard() const {
  return *this;
}

#pragma once

#include "../model/Edge.hpp"
#include "../model/EdgeCountOfBox.hpp"
#include "../model/NearEdges.hpp"
#include "../model/Square.hpp"
#include "BasicBoard.hpp"

template <int64_t BoardSize>
class EdgeCountableBoard {
  public:
  EdgeCountableBoard() = default;

  SizeType<BoardSize>
  Add(const Edge<BoardSize> edge);
  Edge<BoardSize>
  FindNotContainsEdgeInBox(const Box<BoardSize> box) const;
  Edge<BoardSize>
  FindScoreableEdge() const;
  const BasicBoard<BoardSize>&
  GetBasicBoard() const;
  const EdgeCountOfBox<BoardSize>&
  GetEdgeCountOfBox() const;

  private:
  BasicBoard<BoardSize> BasicBoard;
  EdgeCountOfBox<BoardSize> EdgeCountOfBox;
};

template <int64_t BoardSize>
SizeType<BoardSize>
EdgeCountableBoard<BoardSize>::Add(const Edge<BoardSize> edge) {
  BasicBoard.Add(edge);
  return EdgeCountOfBox.Add(edge);
}

template <int64_t BoardSize>
Edge<BoardSize>
EdgeCountableBoard<BoardSize>::FindNotContainsEdgeInBox(const Box<BoardSize> box) const {
  assert(EdgeCountOfBox.EdgeCount(box) == 3);
  for (const Edge<BoardSize> edge : NearEdges(box)) {
    if (!BasicBoard.Contains(edge)) {
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
    if (EdgeCountOfBox.EdgeCount(box) == 3) {
      return FindNotContainsEdgeInBox(box);
    }
  }
  return InvalidEdge<BoardSize>();
}

template <int64_t BoardSize>
const BasicBoard<BoardSize>&
EdgeCountableBoard<BoardSize>::GetBasicBoard() const {
  return BasicBoard;
}

template <int64_t BoardSize>
const EdgeCountOfBox<BoardSize>&
EdgeCountableBoard<BoardSize>::GetEdgeCountOfBox() const {
  return EdgeCountOfBox;
}

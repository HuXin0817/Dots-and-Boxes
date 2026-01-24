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
  Add(const Edge<BoardSize> edge) {
    BasicBoard.Add(edge);
    return EdgeCountOfBox.Add(edge);
  }

  Edge<BoardSize>
  FindNotContainsEdgeInBox(const Box<BoardSize> box) const {
    assert(EdgeCountOfBox.EdgeCount(box) == 3);
    for (const Edge<BoardSize> edge : NearEdges(box)) {
      if (BasicBoard.NotContains(edge)) {
        return edge;
      }
    }
    assert(false);
    return InvalidEdge<BoardSize>();
  }

  Edge<BoardSize>
  FindScoreableEdge() const {
    for (const Box<BoardSize> box : Iota(Box<BoardSize>::Max)) {
      if (EdgeCountOfBox.EdgeCount(box) == 3) {
        return FindNotContainsEdgeInBox(box);
      }
    }
    return InvalidEdge<BoardSize>();
  }

  const BasicBoard<BoardSize>&
  GetBasicBoard() const {
    return BasicBoard;
  }

  const EdgeCountOfBox<BoardSize>&
  GetEdgeCountOfBox() const {
    return EdgeCountOfBox;
  }

  private:
  BasicBoard<BoardSize> BasicBoard;
  EdgeCountOfBox<BoardSize> EdgeCountOfBox;
};

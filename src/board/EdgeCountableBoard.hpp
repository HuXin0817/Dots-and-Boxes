#pragma once

#include "../model/Edge.hpp"
#include "../model/EdgeCountOfBox.hpp"
#include "../model/NearEdges.hpp"
#include "../model/Square.hpp"
#include "BasicBoard.hpp"

template <int BoardSize, typename SizeType>
class EdgeCountableBoard {
  public:
  EdgeCountableBoard() = default;

  SizeType
  Add(const Edge<BoardSize, SizeType> edge) {
    BasicBoard.Add(edge);
    return EdgeCountOfBox.Add(edge);
  }

  Edge<BoardSize, SizeType>
  FindNotContainsEdgeInBox(const Box<BoardSize, SizeType> box) const {
    assert(EdgeCountOfBox.EdgeCount(box) == 3);
    for (const Edge<BoardSize, SizeType> edge : NearEdges(box)) {
      if (BasicBoard.NotContains(edge)) {
        return edge;
      }
    }
    assert(false);
    return InvalidEdge<BoardSize, SizeType>();
  }

  Edge<BoardSize, SizeType>
  FindScoreableEdge() const {
    for (const Box<BoardSize, SizeType> box : ValueIterator<Box<BoardSize, SizeType>, SizeType>()) {
      if (EdgeCountOfBox.EdgeCount(box) == 3) {
        return FindNotContainsEdgeInBox(box);
      }
    }
    return InvalidEdge<BoardSize, SizeType>();
  }

  const BasicBoard<BoardSize, SizeType>&
  GetBasicBoard() const {
    return BasicBoard;
  }

  const EdgeCountOfBox<BoardSize, SizeType>&
  GetEdgeCountOfBox() const {
    return EdgeCountOfBox;
  }

  private:
  BasicBoard<BoardSize, SizeType> BasicBoard;
  EdgeCountOfBox<BoardSize, SizeType> EdgeCountOfBox;
};

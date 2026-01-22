#pragma once

#include "../model/Edge.hpp"
#include "../model/EdgeCountOfBox.hpp"
#include "../model/NearEdges.hpp"
#include "../model/Square.hpp"
#include "BasicBoard.hpp"

namespace dab::board {

template <int BoardSize, typename SizeType>
class EdgeCountableBoard {
  public:
  EdgeCountableBoard() = default;

  int
  Add(const model::Edge<BoardSize, SizeType> edge) {
    BasicBoard.Add(edge);
    return EdgeCountOfBox.Add(edge);
  }

  model::Edge<BoardSize, SizeType>
  FindNotContainsEdgeInBox(const model::Box<BoardSize, SizeType> box) const {
    assert(EdgeCountOfBox.EdgeCount(box) == 3);
    for (const model::Edge<BoardSize, SizeType> edge : model::NearEdges(box)) {
      if (BasicBoard.NotContains(edge)) {
        return edge;
      }
    }
    assert(false);
    return model::InvalidEdge<BoardSize, SizeType>();
  }

  model::Edge<BoardSize, SizeType>
  FindScoreableEdge() const {
    for (const model::Box<BoardSize, SizeType> box : model::ValueIterator<model::Box<BoardSize, SizeType>>()) {
      if (EdgeCountOfBox.EdgeCount(box) == 3) {
        return FindNotContainsEdgeInBox(box);
      }
    }
    return model::InvalidEdge<BoardSize, SizeType>();
  }

  const BasicBoard<BoardSize, SizeType>&
  GetBasicBoard() const {
    return BasicBoard;
  }

  const model::EdgeCountOfBox<BoardSize, SizeType>&
  GetEdgeCountOfBox() const {
    return EdgeCountOfBox;
  }

  private:
  BasicBoard<BoardSize, SizeType> BasicBoard;
  model::EdgeCountOfBox<BoardSize, SizeType> EdgeCountOfBox;
};

}  // namespace dab::board

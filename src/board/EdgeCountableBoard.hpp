#pragma once

#include "../model/Edge.hpp"
#include "../model/EdgeCountOfBox.hpp"
#include "../model/NearEdges.hpp"
#include "../model/Square.hpp"
#include "BasicBoard.hpp"

namespace dab::board {

template <int BoardSize>
class EdgeCountableBoard {
  public:
  EdgeCountableBoard() = default;

  int
  Add(const model::Edge<BoardSize> edge) {
    BasicBoard.Add(edge);
    return EdgeCountOfBox.Add(edge);
  }

  model::Edge<BoardSize>
  FindNotContainsEdgeInBox(const model::Box<BoardSize> box) const {
    assert(EdgeCountOfBox.EdgeCount(box) == 3);
    for (const model::Edge<BoardSize> edge : model::NearEdges(box)) {
      if (BasicBoard.NotContains(edge)) {
        return edge;
      }
    }
    assert(false);
    return model::InvalidEdge<BoardSize>();
  }

  model::Edge<BoardSize>
  FindScoreableEdge() const {
    for (model::Box<BoardSize> box = 0; box.Value() < model::Box<BoardSize>::Max; ++box) {
      if (EdgeCountOfBox.EdgeCount(box) == 3) {
        return FindNotContainsEdgeInBox(box);
      }
    }
    return model::InvalidEdge<BoardSize>();
  }

  const BasicBoard<BoardSize>&
  GetBasicBoard() const {
    return BasicBoard;
  }

  const model::EdgeCountOfBox<BoardSize>&
  GetEdgeCountOfBox() const {
    return EdgeCountOfBox;
  }

  private:
  BasicBoard<BoardSize> BasicBoard;
  model::EdgeCountOfBox<BoardSize> EdgeCountOfBox;
};

}  // namespace dab::board

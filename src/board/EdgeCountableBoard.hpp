#pragma once

#include "../model/Edge.hpp"
#include "../model/EdgeCountOfBox.hpp"
#include "../model/NearEdges.hpp"
#include "../model/Square.hpp"
#include "BasicBoard.hpp"

template <int BoardSize>
class EdgeCountableBoard {
  public:
  EdgeCountableBoard() = default;

  int
  Add(Edge<BoardSize> edge) {
    BasicBoard.Add(edge);
    return EdgeCountOfBox.Add(edge);
  }

  Edge<BoardSize>
  FindNotContainsEdgeInBox(Box<BoardSize> box) const {
    assert(EdgeCountOfBox.At(box) == 3);
    for (Edge<BoardSize> edge : NearEdges(box)) {
      if (BasicBoard.NotContains(edge)) {
        return edge;
      }
    }
    assert(false);
    return -1;
  }

  Edge<BoardSize>
  FindScoreableEdge() const {
    for (Box<BoardSize> box = 0; box < Box<BoardSize>::Max; box++) {
      if (EdgeCountOfBox.At(box) == 3) {
        return FindNotContainsEdgeInBox(box);
      }
    }
    return -1;
  }

  EdgeCountableBoard&
  GetEdgeCountableBoard() {
    return *this;
  }

  const EdgeCountableBoard&
  GetEdgeCountableBoard() const {
    return *this;
  }

  const BasicBoard<BoardSize>&
  GetBasicBoard() const {
    return BasicBoard;
  }

  BasicBoard<BoardSize>&
  GetBasicBoard() {
    return BasicBoard;
  }

  const EdgeCountOfBox<BoardSize>&
  GetEdgeCountOfBox() const {
    return EdgeCountOfBox;
  }

  EdgeCountOfBox<BoardSize>&
  GetEdgeCountOfBox() {
    return EdgeCountOfBox;
  }

  private:
  BasicBoard<BoardSize> BasicBoard;
  EdgeCountOfBox<BoardSize> EdgeCountOfBox;
};

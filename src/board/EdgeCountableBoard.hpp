#pragma once

#include "../model/Edge.hpp"
#include "../model/EdgeCountOfBox.hpp"
#include "../model/Square.hpp"
#include "BasicBoard.hpp"

class EdgeCountableBoard : public BasicBoard, public EdgeCountOfBox {
  public:
  EdgeCountableBoard() = default;

  int
  Add(Edge edge) {
    BasicBoard::Add(edge);
    return EdgeCountOfBox::Add(edge);
  }

  Edge
  FindNotContainsEdgeInBox(Box box) const {
    assert(EdgeCountOfBox::At(box) == 3);
    for (Edge edge : EdgeBoxMapper::BoxNearEdges.At(box)) {
      if (NotContains(edge)) {
        return edge;
      }
    }
    assert(false);
    return -1;
  }

  Edge
  FindScoreableEdge() const {
    for (Box box = 0; box < Box::Max; box++) {
      if (EdgeCountOfBox::At(box) == 3) {
        return FindNotContainsEdgeInBox(box);
      }
    }
    return -1;
  }

  EdgeCountableBoard&
  GetBoardV1() {
    return *this;
  }

  const EdgeCountableBoard&
  GetBoardV1() const {
    return *this;
  }
};

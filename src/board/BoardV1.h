#pragma once

#include "../model/Edge.h"
#include "../model/EdgeCountOfBox.h"
#include "../model/Square.h"
#include "BoardV0.h"

class BoardV1 : public BoardV0, public EdgeCountOfBox {
  public:
  BoardV1() = default;

  int
  Add(Edge edge) {
    BoardV0::Add(edge);
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

  BoardV1&
  GetBoardV1() {
    return *this;
  }

  const BoardV1&
  GetBoardV1() const {
    return *this;
  }
};

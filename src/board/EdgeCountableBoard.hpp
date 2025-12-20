#pragma once

#include "../model/Edge.hpp"
#include "../model/EdgeCountOfBox.hpp"
#include "../model/NearEdges.hpp"
#include "../model/Square.hpp"
#include "BasicBoard.hpp"

template <int BoardSize>
class EdgeCountableBoard : public BasicBoard<BoardSize>, public EdgeCountOfBox<BoardSize> {
  public:
  EdgeCountableBoard() = default;

  int
  Add(Edge<BoardSize> edge) {
    BasicBoard<BoardSize>::Add(edge);
    return EdgeCountOfBox<BoardSize>::Add(edge);
  }

  Edge<BoardSize>
  FindNotContainsEdgeInBox(Box<BoardSize> box) const {
    assert(EdgeCountOfBox<BoardSize>::At(box) == 3);
    for (Edge edge : NearEdges(box)) {
      if (BasicBoard<BoardSize>::NotContains(edge)) {
        return edge;
      }
    }
    assert(false);
    return -1;
  }

  Edge<BoardSize>
  FindScoreableEdge() const {
    for (Box<BoardSize> box = 0; box < Box<BoardSize>::Max; box++) {
      if (EdgeCountOfBox<BoardSize>::At(box) == 3) {
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
};

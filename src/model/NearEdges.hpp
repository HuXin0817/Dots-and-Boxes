#pragma once

#include "../common/Array.hpp"
#include "Edge.hpp"
#include "Square.hpp"

namespace dab::model {

template <int BoardSize>
const common::Array<Edge<BoardSize>, 4>&
NearEdges(const Box<BoardSize> box);

template <int BoardSize>
class NearEdgesMapper {
  public:
  NearEdgesMapper() {
    for (int x = 0; x < Box<BoardSize>::Size; x++) {
      for (int y = 0; y < Box<BoardSize>::Size; y++) {
        Dot<BoardSize> topLeft(x, y);
        Dot<BoardSize> topRight(x + 1, y);
        Dot<BoardSize> bottomLeft(x, y + 1);
        Dot<BoardSize> bottomRight(x + 1, y + 1);
        BoxNearEdges.At(Box<BoardSize>(x, y).Value()) = {
            Edge(topLeft, topRight),
            Edge(topLeft, bottomLeft),
            Edge(bottomLeft, bottomRight),
            Edge(topRight, bottomRight),
        };
      }
    }
  }

  private:
  common::Array<common::Array<Edge<BoardSize>, 4>, Box<BoardSize>::Max> BoxNearEdges;

  friend const common::Array<Edge<BoardSize>, 4>&
  NearEdges<BoardSize>(const Box<BoardSize> box);
};

template <int BoardSize>
const common::Array<Edge<BoardSize>, 4>&
NearEdges(const Box<BoardSize> box) {
  static NearEdgesMapper<BoardSize> NearEdgesMapperInstance;

  return NearEdgesMapperInstance.BoxNearEdges.At(box.Value());
}

}  // namespace dab::model

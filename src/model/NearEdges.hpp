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
        const Dot<BoardSize> topLeft(x, y);
        const Dot<BoardSize> topRight(x + 1, y);
        const Dot<BoardSize> bottomLeft(x, y + 1);
        const Dot<BoardSize> bottomRight(x + 1, y + 1);
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

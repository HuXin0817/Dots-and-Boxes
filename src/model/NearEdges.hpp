#pragma once

#include "../common/Array.hpp"
#include "../common/Iota.hpp"
#include "Edge.hpp"
#include "Square.hpp"

template <int BoardSize>
const Array<Edge<BoardSize>, 4>&
NearEdges(const Box<BoardSize> box);

template <int BoardSize>
class NearEdgesMapper {
  public:
  NearEdgesMapper() {
    for (const SizeType x : Iota(Box<BoardSize>::Size)) {
      for (const SizeType y : Iota(Box<BoardSize>::Size)) {
        const Dot<BoardSize> topLeft(x, y);
        const Dot<BoardSize> topRight(x + 1, y);
        const Dot<BoardSize> bottomLeft(x, y + 1);
        const Dot<BoardSize> bottomRight(x + 1, y + 1);
        const Box<BoardSize> box(x, y);
        BoxNearEdges.At(box.Value()).At(0) = Edge(topLeft, topRight);
        BoxNearEdges.At(box.Value()).At(1) = Edge(topLeft, bottomLeft);
        BoxNearEdges.At(box.Value()).At(2) = Edge(bottomLeft, bottomRight);
        BoxNearEdges.At(box.Value()).At(3) = Edge(topRight, bottomRight);
      }
    }
  }

  private:
  Array<Array<Edge<BoardSize>, 4>, Box<BoardSize>::Max> BoxNearEdges;

  friend const Array<Edge<BoardSize>, 4>&
  NearEdges<BoardSize>(const Box<BoardSize> box);
};

template <int BoardSize>
const Array<Edge<BoardSize>, 4>&
NearEdges(const Box<BoardSize> box) {
  static NearEdgesMapper<BoardSize> Instance;

  return Instance.BoxNearEdges.At(box.Value());
}

#pragma once

#include "../common/Array.hpp"
#include "Edge.hpp"
#include "Square.hpp"

template <int64_t BoardSize>
const Array<Edge<BoardSize>, 4>&
NearEdges(const Box<BoardSize> box);

template <int64_t BoardSize>
class NearEdgesMapper {
  public:
  NearEdgesMapper() {
    for (const SizeType x : std::views::iota(0, Box<BoardSize>::Size)) {
      for (const SizeType y : std::views::iota(0, Box<BoardSize>::Size)) {
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

template <int64_t BoardSize>
const Array<Edge<BoardSize>, 4>&
NearEdges(const Box<BoardSize> box) {
  static NearEdgesMapper<BoardSize> NearEdgesMapperInstance;

  return NearEdgesMapperInstance.BoxNearEdges.At(box.Value());
}

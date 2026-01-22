#pragma once

#include "../common/Array.hpp"
#include "Edge.hpp"
#include "Square.hpp"

template <int BoardSize, typename SizeType>
const Array<Edge<BoardSize, SizeType>, 4, SizeType>&
NearEdges(const Box<BoardSize, SizeType> box);

template <int BoardSize, typename SizeType>
class NearEdgesMapper {
  public:
  NearEdgesMapper() {
    for (SizeType x = 0; x < Box<BoardSize, SizeType>::Size; x++) {
      for (SizeType y = 0; y < Box<BoardSize, SizeType>::Size; y++) {
        const Dot<BoardSize, SizeType> topLeft(x, y);
        const Dot<BoardSize, SizeType> topRight(x + 1, y);
        const Dot<BoardSize, SizeType> bottomLeft(x, y + 1);
        const Dot<BoardSize, SizeType> bottomRight(x + 1, y + 1);
        const Box<BoardSize, SizeType> box(x, y);
        BoxNearEdges.At(box.Value()).At(0) = Edge(topLeft, topRight);
        BoxNearEdges.At(box.Value()).At(1) = Edge(topLeft, bottomLeft);
        BoxNearEdges.At(box.Value()).At(2) = Edge(bottomLeft, bottomRight);
        BoxNearEdges.At(box.Value()).At(3) = Edge(topRight, bottomRight);
      }
    }
  }

  private:
  Array<Array<Edge<BoardSize, SizeType>, 4, SizeType>, Box<BoardSize, SizeType>::Max, SizeType> BoxNearEdges;

  friend const Array<Edge<BoardSize, SizeType>, 4, SizeType>&
  NearEdges<BoardSize, SizeType>(const Box<BoardSize, SizeType> box);
};

template <int BoardSize, typename SizeType>
const Array<Edge<BoardSize, SizeType>, 4, SizeType>&
NearEdges(const Box<BoardSize, SizeType> box) {
  static NearEdgesMapper<BoardSize, SizeType> NearEdgesMapperInstance;

  return NearEdgesMapperInstance.BoxNearEdges.At(box.Value());
}

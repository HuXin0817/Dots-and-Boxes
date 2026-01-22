#pragma once

#include "../common/Array.hpp"
#include "Edge.hpp"
#include "Square.hpp"

namespace dab::model {

template <int BoardSize, typename SizeType>
const common::Array<Edge<BoardSize, SizeType>, 4, int>&
NearEdges(const Box<BoardSize, SizeType> box);

template <int BoardSize, typename SizeType>
class NearEdgesMapper {
  public:
  NearEdgesMapper() {
    for (int x = 0; x < Box<BoardSize, SizeType>::Size; x++) {
      for (int y = 0; y < Box<BoardSize, SizeType>::Size; y++) {
        const Dot<BoardSize, SizeType> topLeft(x, y);
        const Dot<BoardSize, SizeType> topRight(x + 1, y);
        const Dot<BoardSize, SizeType> bottomLeft(x, y + 1);
        const Dot<BoardSize, SizeType> bottomRight(x + 1, y + 1);
        BoxNearEdges.At(Box<BoardSize, SizeType>(x, y).Value()) = {
            Edge(topLeft, topRight),
            Edge(topLeft, bottomLeft),
            Edge(bottomLeft, bottomRight),
            Edge(topRight, bottomRight),
        };
      }
    }
  }

  private:
  common::Array<common::Array<Edge<BoardSize, SizeType>, 4, int>, Box<BoardSize, SizeType>::Max, int> BoxNearEdges;

  friend const common::Array<Edge<BoardSize, SizeType>, 4, int>&
  NearEdges<BoardSize, SizeType>(const Box<BoardSize, SizeType> box);
};

template <int BoardSize, typename SizeType>
const common::Array<Edge<BoardSize, SizeType>, 4, int>&
NearEdges(const Box<BoardSize, SizeType> box) {
  static NearEdgesMapper<BoardSize, SizeType> NearEdgesMapperInstance;

  return NearEdgesMapperInstance.BoxNearEdges.At(box.Value());
}

}  // namespace dab::model

#pragma once

#include "../common/Array.hpp"
#include "Edge.hpp"
#include "Square.hpp"

namespace dab {

template <int64_t BoardSize>
const Array<Edge<BoardSize>, 4>&
NearEdges(const Box<BoardSize> box);

template <int64_t BoardSize>
class NearEdgesMapper {
  public:
  NearEdgesMapper();

  private:
  Array<Array<Edge<BoardSize>, 4>, Limits<Box<BoardSize>>::Max> BoxNearEdges;

  static Array<Edge<BoardSize>, 4>
  GetNearEdges(const Box<BoardSize> box);

  friend const Array<Edge<BoardSize>, 4>&
  NearEdges<BoardSize>(const Box<BoardSize> box);
};

template <int64_t BoardSize>
NearEdgesMapper<BoardSize>::NearEdgesMapper() {
  for (const Box<BoardSize> box : Iota<Box<BoardSize>>()) {
    BoxNearEdges[box] = GetNearEdges(box);
  }
}

template <int64_t BoardSize>
Array<Edge<BoardSize>, 4>
NearEdgesMapper<BoardSize>::GetNearEdges(const Box<BoardSize> box) {
  Array<Edge<BoardSize>, 4> NearEdges;
  const SizeType<BoardSize> x = box.X();
  const SizeType<BoardSize> y = box.Y();
  const Dot<BoardSize> topLeft(x, y);
  const Dot<BoardSize> topRight(x + 1, y);
  const Dot<BoardSize> bottomLeft(x, y + 1);
  const Dot<BoardSize> bottomRight(x + 1, y + 1);
  NearEdges[0] = Edge(topLeft, topRight);
  NearEdges[1] = Edge(topLeft, bottomLeft);
  NearEdges[2] = Edge(bottomLeft, bottomRight);
  NearEdges[3] = Edge(topRight, bottomRight);
  return NearEdges;
}

template <int64_t BoardSize>
const Array<Edge<BoardSize>, 4>&
NearEdges(const Box<BoardSize> box) {
  static NearEdgesMapper<BoardSize> Instance;

  return Instance.BoxNearEdges[box];
}

}  // namespace dab

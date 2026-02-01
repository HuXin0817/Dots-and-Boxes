#pragma once

#include "Common.hpp"
#include "Edge.hpp"
#include "Square.hpp"

namespace dab::detail::model {

template <int64_t BoardSize>
const Array<Edge<BoardSize>, 4>&
NearEdges(const Box<BoardSize> box);

template <int64_t BoardSize>
class NearEdgesMapper {
  public:
  constexpr NearEdgesMapper();

  private:
  Array<Array<Edge<BoardSize>, 4>, Box<BoardSize>::Max> BoxNearEdges;

  static constexpr Array<Edge<BoardSize>, 4>
  GetNearEdges(const Box<BoardSize> box);

  friend const Array<Edge<BoardSize>, 4>&
  NearEdges<BoardSize>(const Box<BoardSize> box);
};

template <int64_t BoardSize>
constexpr NearEdgesMapper<BoardSize>::NearEdgesMapper() {
  for (Box<BoardSize> box = 0; box < Box<BoardSize>::Max; ++box) {
    BoxNearEdges[box] = GetNearEdges(box);
  }
}

template <int64_t BoardSize>
constexpr Array<Edge<BoardSize>, 4>
NearEdgesMapper<BoardSize>::GetNearEdges(const Box<BoardSize> box) {
  Array<Edge<BoardSize>, 4> NearEdges;
  const Int<BoardSize> x = box.X();
  const Int<BoardSize> y = box.Y();
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
  static constexpr NearEdgesMapper<BoardSize> Instance;

  return Instance.BoxNearEdges[box];
}

}  // namespace dab::detail::model
#pragma once

#include <cassert>

#include "Square.hpp"
#include "Types.hpp"

namespace dab {

template <int64_t BoardSize>
class Edge : public SizeTypeWapper<BoardSize> {
  public:
  using SizeTypeWapper<BoardSize>::SizeTypeWapper;
  constexpr Edge(const Dot<BoardSize> dot1, const Dot<BoardSize> dot2);

  constexpr Dot<BoardSize>
  Dot1() const;
  constexpr Dot<BoardSize>
  Dot2() const;
  constexpr bool
  Rotate() const;
};

template <int64_t BoardSize>
constexpr Edge<BoardSize>::Edge(const Dot<BoardSize> dot1, const Dot<BoardSize> dot2) {
  if (dot2 - dot1 == 1) {
    SizeTypeWapper<BoardSize>::v = 2 * (dot1 - dot1 / (BoardSize + 1)) + 1;
  } else {
    SizeTypeWapper<BoardSize>::v = 2 * dot1;
  }

  assert(Dot1() == dot1);
  assert(Dot2() == dot2);
}

template <int64_t BoardSize>
constexpr Dot<BoardSize>
Edge<BoardSize>::Dot1() const {
  SizeType<BoardSize> dot = SizeTypeWapper<BoardSize>::v >> 1;
  if (SizeTypeWapper<BoardSize>::v & 1) {
    dot += dot / BoardSize;
  }
  return dot;
}

template <int64_t BoardSize>
constexpr Dot<BoardSize>
Edge<BoardSize>::Dot2() const {
  SizeType<BoardSize> dot = SizeTypeWapper<BoardSize>::v >> 1;
  if (SizeTypeWapper<BoardSize>::v & 1) {
    dot += dot / BoardSize + 1;
  } else {
    dot += BoardSize + 1;
  }
  return dot;
}

template <int64_t BoardSize>
constexpr bool
Edge<BoardSize>::Rotate() const {
  return SizeTypeWapper<BoardSize>::v & 1;
}

template <int64_t BoardSize>
constexpr Edge<BoardSize>
InvalidEdge() {
  return -1;
}

template <int64_t BoardSize>
class Limits<Edge<BoardSize>> {
  public:
  static constexpr Edge<BoardSize> Max = 2 * BoardSize * (BoardSize + 1);
};

}  // namespace dab

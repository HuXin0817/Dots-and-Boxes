#pragma once

#include <cassert>

#include "Square.hpp"
#include "Types.hpp"

namespace dab::detail::model {

template <int64_t BoardSize>
class Edge : public IntWapper<BoardSize> {
  public:
  using IntWapper<BoardSize>::IntWapper;
  constexpr Edge(const Dot<BoardSize> dot1, const Dot<BoardSize> dot2);

  static constexpr Int<BoardSize> Max = 2 * BoardSize * (BoardSize + 1);

  constexpr Dot<BoardSize> Dot1() const;
  constexpr Dot<BoardSize> Dot2() const;
  constexpr bool Rotate() const;
};

template <int64_t BoardSize>
constexpr Edge<BoardSize>::Edge(const Dot<BoardSize> dot1, const Dot<BoardSize> dot2) {
  if (dot2 - dot1 == 1) {
    IntWapper<BoardSize>::v = 2 * (dot1 - dot1 / (BoardSize + 1)) + 1;
  } else {
    IntWapper<BoardSize>::v = 2 * dot1;
  }

  assert(Dot1() == dot1);
  assert(Dot2() == dot2);
}

template <int64_t BoardSize>
constexpr Dot<BoardSize> Edge<BoardSize>::Dot1() const {
  Int<BoardSize> dot = IntWapper<BoardSize>::v >> 1;
  if (IntWapper<BoardSize>::v & 1) {
    dot += dot / BoardSize;
  }
  return dot;
}

template <int64_t BoardSize>
constexpr Dot<BoardSize> Edge<BoardSize>::Dot2() const {
  Int<BoardSize> dot = IntWapper<BoardSize>::v >> 1;
  if (IntWapper<BoardSize>::v & 1) {
    dot += dot / BoardSize + 1;
  } else {
    dot += BoardSize + 1;
  }
  return dot;
}

template <int64_t BoardSize>
constexpr bool Edge<BoardSize>::Rotate() const {
  return IntWapper<BoardSize>::v & 1;
}

template <int64_t BoardSize>
static constexpr Edge<BoardSize> InvalidEdge = -1;

}  // namespace dab::detail::model
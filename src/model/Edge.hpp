#pragma once

#include <cassert>

#include "Square.hpp"
#include "Types.hpp"

namespace dab {

template <int64_t BoardSize>
class Edge {
  public:
  Edge() = default;
  Edge(SizeType<BoardSize> v);
  Edge(const Dot<BoardSize> dot1, const Dot<BoardSize> dot2);

  operator SizeType<BoardSize>() const;
  Dot<BoardSize>
  Dot1() const;
  Dot<BoardSize>
  Dot2() const;
  bool
  Rotate() const;

  private:
  SizeType<BoardSize> v;
};

template <int64_t BoardSize>
Edge<BoardSize>::Edge(SizeType<BoardSize> v) : v(v) {
}

template <int64_t BoardSize>
Edge<BoardSize>::Edge(const Dot<BoardSize> dot1, const Dot<BoardSize> dot2) {
  if (dot2 - dot1 == 1) {
    v = 2 * (dot1 - dot1 / (BoardSize + 1)) + 1;
  } else {
    v = 2 * dot1;
  }

  assert(Dot1() == dot1);
  assert(Dot2() == dot2);
}

template <int64_t BoardSize>
Edge<BoardSize>::
operator SizeType<BoardSize>() const {
  return v;
}

template <int64_t BoardSize>
Dot<BoardSize>
Edge<BoardSize>::Dot1() const {
  SizeType<BoardSize> dot = v >> 1;
  if (v & 1) {
    dot += dot / BoardSize;
  }
  return dot;
}

template <int64_t BoardSize>
Dot<BoardSize>
Edge<BoardSize>::Dot2() const {
  SizeType<BoardSize> dot = v >> 1;
  if (v & 1) {
    dot += dot / BoardSize + 1;
  } else {
    dot += BoardSize + 1;
  }
  return dot;
}

template <int64_t BoardSize>
bool
Edge<BoardSize>::Rotate() const {
  return v & 1;
}

template <int64_t BoardSize>
Edge<BoardSize>
InvalidEdge() {
  return -1;
}

template <int64_t BoardSize>
class Limits<Edge<BoardSize>> {
  public:
  static constexpr SizeType<BoardSize> Max = 2 * BoardSize * (BoardSize + 1);
};

}  // namespace dab

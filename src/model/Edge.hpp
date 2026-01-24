#pragma once

#include <cassert>

#include "Square.hpp"

template <int64_t BoardSize>
class Edge {
  public:
  static constexpr SizeType<BoardSize> Max = 2 * BoardSize * (BoardSize + 1);

  Edge() = default;
  Edge(SizeType<BoardSize> v);
  Edge(const Dot<BoardSize> dot1, const Dot<BoardSize> dot2);

  SizeType<BoardSize>
  Value() const;
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
  if (dot2.Value() - dot1.Value() == 1) {
    v = 2 * (dot1.Value() - dot1.Value() / (BoardSize + 1)) + 1;
  } else {
    v = 2 * dot1.Value();
  }

  assert(Dot1().Value() == dot1.Value());
  assert(Dot2().Value() == dot2.Value());
}

template <int64_t BoardSize>
SizeType<BoardSize>
Edge<BoardSize>::Value() const {
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

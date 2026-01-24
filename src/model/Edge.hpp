#pragma once

#include "Square.hpp"

template <int64_t BoardSize>
class Edge {
  public:
  static constexpr Int<BoardSize> Max = 2 * BoardSize * (BoardSize + 1);

  Edge() = default;

  Edge(Int<BoardSize> v) : v(v) {
  }

  Edge(const Dot<BoardSize> dot1, const Dot<BoardSize> dot2) {
    if (dot2.Value() - dot1.Value() == 1) {
      v = 2 * (dot1.Value() - dot1.Value() / (BoardSize + 1)) + 1;
    } else {
      v = 2 * dot1.Value();
    }

    assert(Dot1().Value() == dot1.Value());
    assert(Dot2().Value() == dot2.Value());
  }

  Int<BoardSize>
  Value() const {
    return v;
  }

  Dot<BoardSize>
  Dot1() const {
    Int<BoardSize> dot = v >> 1;
    if (v & 1) {
      dot += dot / BoardSize;
    }
    return dot;
  }

  Dot<BoardSize>
  Dot2() const {
    Int<BoardSize> dot = v >> 1;
    if (v & 1) {
      dot += dot / BoardSize + 1;
    } else {
      dot += BoardSize + 1;
    }
    return dot;
  }

  bool
  Rotate() const {
    return v & 1;
  }

  private:
  Int<BoardSize> v;
};

template <int64_t BoardSize>
Edge<BoardSize>
InvalidEdge() {
  return -1;
}

#pragma once

#include "Dot.hpp"

template <int BoardSize>
class Edge {
  public:
  static constexpr int Max = 2 * BoardSize * (BoardSize + 1);

  Edge(Dot<BoardSize> dot1, Dot<BoardSize> dot2) {
    if (dot2 - dot1 == 1) {
      v = 2 * (dot1 - dot1 / (BoardSize + 1)) + 1;
    } else {
      v = 2 * dot1;
    }

    assert(Dot1() == dot1);
    assert(Dot2() == dot2);
  }

  Dot<BoardSize>
  Dot1() const {
    int dot = v >> 1;
    if (v & 1) {
      dot += dot / BoardSize;
    }
    return dot;
  }

  Dot<BoardSize>
  Dot2() const {
    int dot = v >> 1;
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

  operator std::string() const {
    std::stringstream ss;
    ss << std::string(Dot1()) << " -> " << std::string(Dot2());
    return ss.str();
  }

  V(Edge<BoardSize>)
};

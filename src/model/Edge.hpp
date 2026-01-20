#pragma once

#include "Square.hpp"

namespace dab::model {

template <int BoardSize>
class Edge {
  public:
  static constexpr int Max = 2 * BoardSize * (BoardSize + 1);

  Edge(const Dot<BoardSize> dot1, const Dot<BoardSize> dot2) {
    if (dot2.Value() - dot1.Value() == 1) {
      v = 2 * (dot1.Value() - dot1.Value() / (BoardSize + 1)) + 1;
    } else {
      v = 2 * dot1.Value();
    }

    assert(Dot1().Value() == dot1.Value());
    assert(Dot2().Value() == dot2.Value());
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

  explicit
  operator std::string() const {
    std::stringstream ss;
    ss << std::string(Dot1()) << " -> " << std::string(Dot2());
    return ss.str();
  }

  IntValueObject(Edge<BoardSize>)
};

template <int BoardSize>
Edge<BoardSize>
InvalidEdge() {
  return -1;
}

}  // namespace dab::model

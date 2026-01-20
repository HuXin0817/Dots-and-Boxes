#pragma once

#include "Edge.hpp"

namespace dab::model {

template <int BoardSize>
class Step {
  public:
  void
  Reset() {
    v = 0;
  }

  bool
  Gaming() const {
    return v < Edge<BoardSize>::Max;
  }

  int
  RemainStep() const {
    return Edge<BoardSize>::Max - v;
  }

  int
  NowStep() const {
    return v;
  }

  void
  Go() {
    v++;
  }

  private:
  int v = 0;
};

}  // namespace dab::model

#pragma once

#include "Edge.hpp"

template <int64_t BoardSize>
class Step {
  public:
  Step() = default;

  void
  Reset() {
    v = 0;
  }

  bool
  Gaming() const {
    return v < Edge<BoardSize>::Max;
  }

  Int<BoardSize>
  RemainStep() const {
    return Edge<BoardSize>::Max - v;
  }

  Int<BoardSize>
  NowStep() const {
    return v;
  }

  void
  Go() {
    v++;
  }

  private:
  Int<BoardSize> v = 0;
};

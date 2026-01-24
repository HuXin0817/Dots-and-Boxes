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

  SizeType<BoardSize>
  RemainStep() const {
    return Edge<BoardSize>::Max - v;
  }

  SizeType<BoardSize>
  NowStep() const {
    return v;
  }

  void
  Go() {
    v++;
  }

  private:
  SizeType<BoardSize> v = 0;
};

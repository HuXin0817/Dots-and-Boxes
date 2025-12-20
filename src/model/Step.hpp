#pragma once

#include "Edge.hpp"

template <int BoardSize>
class Step {
  public:
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

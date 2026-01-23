#pragma once

#include "Edge.hpp"

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

  SizeType
  RemainStep() const {
    return Edge<BoardSize>::Max - v;
  }

  SizeType
  NowStep() const {
    return v;
  }

  void
  Go() {
    v++;
  }

  private:
  SizeType v = 0;
};

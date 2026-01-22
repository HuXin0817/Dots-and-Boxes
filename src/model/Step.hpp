#pragma once

#include "Edge.hpp"

namespace dab::model {

template <int BoardSize, typename SizeType>
class Step {
  public:
  void
  Reset() {
    v = 0;
  }

  bool
  Gaming() const {
    return v < Edge<BoardSize, SizeType>::Max;
  }

  int
  RemainStep() const {
    return Edge<BoardSize, SizeType>::Max - v;
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

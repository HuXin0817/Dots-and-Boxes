#pragma once

#include "Edge.hpp"

namespace dab {

template <int64_t BoardSize>
class Step {
  public:
  Step() = default;

  void
  Reset();
  void
  Add();
  bool
  Gaming() const;
  SizeType<BoardSize>
  RemainStep() const;
  SizeType<BoardSize>
  NowStep() const;

  private:
  SizeType<BoardSize> v = 0;
};

template <int64_t BoardSize>
void
Step<BoardSize>::Reset() {
  v = 0;
}

template <int64_t BoardSize>
void
Step<BoardSize>::Add() {
  v++;
}

template <int64_t BoardSize>
bool
Step<BoardSize>::Gaming() const {
  return v < Edge<BoardSize>::Max;
}

template <int64_t BoardSize>
SizeType<BoardSize>
Step<BoardSize>::RemainStep() const {
  return Edge<BoardSize>::Max - v;
}

template <int64_t BoardSize>
SizeType<BoardSize>
Step<BoardSize>::NowStep() const {
  return v;
}

}  // namespace dab

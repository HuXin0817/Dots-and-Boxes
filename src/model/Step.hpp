#pragma once

#include "Edge.hpp"

namespace dab::detail::model {

template <int64_t BoardSize>
class Step {
  public:
  constexpr Step() = default;

  constexpr void
  Reset();
  constexpr void
  Add();
  constexpr bool
  Gaming() const;
  constexpr SizeType<BoardSize>
  RemainStep() const;
  constexpr SizeType<BoardSize>
  NowStep() const;

  private:
  SizeType<BoardSize> v = 0;
};

template <int64_t BoardSize>
constexpr void
Step<BoardSize>::Reset() {
  v = 0;
}

template <int64_t BoardSize>
constexpr void
Step<BoardSize>::Add() {
  v++;
}

template <int64_t BoardSize>
constexpr bool
Step<BoardSize>::Gaming() const {
  return v < Limits<Edge<BoardSize>>::Max;
}

template <int64_t BoardSize>
constexpr SizeType<BoardSize>
Step<BoardSize>::RemainStep() const {
  return Limits<Edge<BoardSize>>::Max - v;
}

template <int64_t BoardSize>
constexpr SizeType<BoardSize>
Step<BoardSize>::NowStep() const {
  return v;
}

}  // namespace dab::detail::model
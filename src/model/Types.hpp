#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace dab::detail::model {

template <typename T>
class Limits {
  public:
  static constexpr T Max = std::numeric_limits<T>::max();
};

template <int64_t BoardSize,
          int64_t Max = 2 * BoardSize * (BoardSize + 1),
          typename CondInt32 = std::conditional_t<Max <= Limits<int32_t>::Max, int_fast32_t, int_fast64_t>,
          typename CondInt16 = std::conditional_t<Max <= Limits<int16_t>::Max, int_fast16_t, CondInt32>,
          typename CondInt8 = std::conditional_t<Max <= Limits<int8_t>::Max, int_fast8_t, CondInt16>>
using Int = CondInt8;

template <int64_t BoardSize>
class IntWapper {
  public:
  constexpr IntWapper() = default;
  constexpr IntWapper(Int<BoardSize> v);
  constexpr
  operator Int<BoardSize>();
  constexpr
  operator Int<BoardSize>() const;
  constexpr void
  operator++();

  protected:
  Int<BoardSize> v = 0;
};

template <int64_t BoardSize>
constexpr IntWapper<BoardSize>::IntWapper(Int<BoardSize> v) : v(v) {
}

template <int64_t BoardSize>
constexpr IntWapper<BoardSize>::
operator Int<BoardSize>() {
  return v;
}

template <int64_t BoardSize>
constexpr IntWapper<BoardSize>::
operator Int<BoardSize>() const {
  return v;
}

template <int64_t BoardSize>
constexpr void
IntWapper<BoardSize>::operator++() {
  ++v;
}

}  // namespace dab::detail::model
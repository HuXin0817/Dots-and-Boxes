#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>

namespace dab::detail::model {

template <int64_t BoardSize>
constexpr auto
SelectIntType() {
  constexpr int64_t MaxValue = 2 * BoardSize * (BoardSize + 1);

  if constexpr (MaxValue <= std::numeric_limits<int8_t>::max()) {
    return int_fast8_t();
  } else if constexpr (MaxValue <= std::numeric_limits<int16_t>::max()) {
    return int_fast16_t();
  } else if constexpr (MaxValue <= std::numeric_limits<int32_t>::max()) {
    return int_fast32_t();
  } else {
    return int_fast64_t();
  }
}

template <int64_t BoardSize>
using Int = decltype(SelectIntType<BoardSize>());

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
#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace dab {

template <typename T>
class Limits {
  public:
  static constexpr auto Max = std::numeric_limits<T>::max();
};

template <int64_t Max>
using Int = std::conditional_t<Max <= Limits<int8_t>::Max,
                               int8_t,
                               std::conditional_t<Max <= Limits<int16_t>::Max,
                                                  int16_t,
                                                  std::conditional_t<Max <= Limits<int32_t>::Max, int32_t, int64_t>>>;

template <int64_t BoardSize>
using SizeType = Int<2 * BoardSize*(BoardSize + 1)>;

template <int64_t BoardSize>
class SizeTypeWapper {
  public:
  constexpr SizeTypeWapper() = default;
  constexpr SizeTypeWapper(SizeType<BoardSize> v);
  constexpr
  operator SizeType<BoardSize>();
  constexpr
  operator SizeType<BoardSize>() const;
  constexpr void
  operator++();

  protected:
  SizeType<BoardSize> v = 0;
};

template <int64_t BoardSize>
constexpr SizeTypeWapper<BoardSize>::SizeTypeWapper(SizeType<BoardSize> v) : v(v) {
}

template <int64_t BoardSize>
constexpr SizeTypeWapper<BoardSize>::
operator SizeType<BoardSize>() {
  return v;
}

template <int64_t BoardSize>
constexpr SizeTypeWapper<BoardSize>::
operator SizeType<BoardSize>() const {
  return v;
}

template <int64_t BoardSize>
constexpr void
SizeTypeWapper<BoardSize>::operator++() {
  ++v;
}

}  // namespace dab

#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <ranges>
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

template <typename T>
auto
Iota() {
  return std::views::iota(0, Limits<T>::Max);
}

}  // namespace dab

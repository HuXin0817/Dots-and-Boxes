#pragma once

#include <cassert>
#include <cstddef>
#include <limits>
#include <type_traits>

// clang-format off
template <int64_t BoardSize>
using Int = std::conditional_t<2 * BoardSize * (BoardSize + 1) <= std::numeric_limits<int8_t>::max(), int8_t,
  std::conditional_t<2 * BoardSize * (BoardSize + 1) <= std::numeric_limits<int16_t>::max(), int16_t, 
  std::conditional_t<2 * BoardSize * (BoardSize + 1) <= std::numeric_limits<int32_t>::max(), int32_t, int64_t>>>;
// clang-format on

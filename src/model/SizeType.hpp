#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>

// clang-format off
template <int64_t Max>
using Int = std::conditional_t<Max <= std::numeric_limits<int8_t>::max(), int8_t,
  std::conditional_t<Max <= std::numeric_limits<int16_t>::max(), int16_t, 
  std::conditional_t<Max <= std::numeric_limits<int32_t>::max(), int32_t, int64_t>>>;

template <int64_t BoardSize>
using SizeType = Int<2 * BoardSize * (BoardSize + 1)>;
// clang-format on

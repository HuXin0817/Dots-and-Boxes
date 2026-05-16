#pragma once

#include <cstdint>
#include <limits>

#define CAT(x, y) x##y
#define JOIN(x, y) CAT(x, y)
#define __detail__ JOIN(JOIN(JOIN(__detail__, __BoardSize__), x), __BoardSize__)

namespace dab::__detail__ {

using Int = decltype([]() -> auto {
  constexpr std::int64_t MaxValue = 2 * __BoardSize__ * (__BoardSize__ + 1);

  if constexpr (MaxValue <= std::numeric_limits<std::int8_t>::max()) {
    return std::int8_t{};
  } else if constexpr (MaxValue <= std::numeric_limits<std::int16_t>::max()) {
    return std::int16_t{};
  } else if constexpr (MaxValue <= std::numeric_limits<std::int32_t>::max()) {
    return std::int32_t{};
  } else {
    return std::int64_t{};
  }
}());

static constexpr Int BoardSize = __BoardSize__;

}  // namespace dab::__detail__

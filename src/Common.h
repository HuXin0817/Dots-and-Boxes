#pragma once

#include <cstdint>
#include <limits>
#include <type_traits>

#define CAT(x, y) x##y
#define JOIN(x, y) CAT(x, y)
#define __detail__ JOIN(JOIN(JOIN(__detail__, __BoardSize__), x), __BoardSize__)

namespace dab {

namespace __detail__ {

constexpr auto SelectIntType() {
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
}

using Int = decltype(__detail__::SelectIntType());
static constexpr Int BoardSize = __BoardSize__;

}  // namespace __detail__

template <bool Bp, typename T>
using Mixin = std::conditional_t<Bp, T, std::type_identity<T>>;

static constexpr bool HasFlag(int config, int flag) { return (config & flag) != 0; }

}  // namespace dab
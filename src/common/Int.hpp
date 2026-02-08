#pragma once

#include <cstdint>
#include <limits>

namespace dab::detail::common {

constexpr auto SelectIntType() {
  constexpr int64_t MaxValue = 2 * __BoardSize__ * (__BoardSize__ + 1);

  if constexpr (MaxValue <= std::numeric_limits<int8_t>::max()) {
    return static_cast<int8_t>(0);
  } else if constexpr (MaxValue <= std::numeric_limits<int16_t>::max()) {
    return static_cast<int16_t>(0);
  } else if constexpr (MaxValue <= std::numeric_limits<int32_t>::max()) {
    return static_cast<int32_t>(0);
  } else {
    return static_cast<int64_t>(0);
  }
}

using Int = decltype(SelectIntType());

static constexpr Int BoardSize = __BoardSize__;

}  // namespace dab::detail::common
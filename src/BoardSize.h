/*
Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2025 Xin Hu <huxin0817.hx@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#pragma once

#include <cstdint>
#include <limits>

#include "Tools.h"

namespace dab {

namespace __detail__ {

constexpr auto SelectIntType() {
  constexpr int64_t MaxValue = 2ll * __BoardSize__ * (__BoardSize__ + 1ll);

  if constexpr (MaxValue <= std::numeric_limits<int8_t>::max()) {
    return int8_t{};
  } else if constexpr (MaxValue <= std::numeric_limits<int16_t>::max()) {
    return int16_t{};
  } else if constexpr (MaxValue <= std::numeric_limits<int32_t>::max()) {
    return int32_t{};
  } else {
    return int64_t{};
  }
}

using Int = decltype(__detail__::SelectIntType());

constexpr Int operator""_bs(unsigned long long val) {
  Assert(val <= std::numeric_limits<Int>::max());
  return static_cast<Int>(val);
}

}  // namespace __detail__

using __detail__::Int;
static constexpr Int BoardSize = __BoardSize__;

}  // namespace dab
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
#include <type_traits>

#ifndef __BoardSize__
#error "MACRO '__BoardSize__' UNDEFINED"
#else
#define CAT(x, y) x##y
#define JOIN(x, y) CAT(x, y)
#define detail JOIN(JOIN(JOIN(detail, __BoardSize__), x), __BoardSize__)
#endif

namespace dab {

namespace detail {

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

using Int = decltype(detail::SelectIntType());
static constexpr Int BoardSize = __BoardSize__;

}  // namespace detail

template <bool Bp, typename T>
using Mixin = std::conditional_t<Bp, T, std::type_identity<T>>;

static constexpr bool HasFlag(int config, int flag) { return (config & flag) != 0; }

}  // namespace dab
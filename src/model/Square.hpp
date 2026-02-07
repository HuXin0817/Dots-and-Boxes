/*
Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2025 Xin Hu <202219120810@stu.cdut.edu.cn>

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

#include "Types.hpp"

namespace dab::detail::model {

template <int64_t BoardSize, int64_t Length>
class Square : public IntWapper<BoardSize> {
 public:
  static constexpr Int<BoardSize> Max = Length * Length;

  using IntWapper<BoardSize>::IntWapper;
  constexpr Square(Int<BoardSize> x, Int<BoardSize> y) : IntWapper<BoardSize>(x * Length + y) {}

  constexpr Int<BoardSize> X() const { return IntWapper<BoardSize>::v / Length; }
  constexpr Int<BoardSize> Y() const { return IntWapper<BoardSize>::v % Length; }
};

template <int64_t BoardSize>
using Box = Square<BoardSize, BoardSize>;

template <int64_t BoardSize>
using Dot = Square<BoardSize, BoardSize + 1>;

}  // namespace dab::detail::model
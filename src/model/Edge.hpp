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

#include <cassert>

#include "Square.hpp"
#include "Types.hpp"

namespace dab::detail::model {

template <int64_t BoardSize>
class Edge : public IntWapper<BoardSize> {
 public:
  static constexpr Int<BoardSize> Max = 2 * BoardSize * (BoardSize + 1);

  using IntWapper<BoardSize>::IntWapper;
  constexpr Edge(const Dot<BoardSize> dot1, const Dot<BoardSize> dot2);
  constexpr Dot<BoardSize> Dot1() const;
  constexpr Dot<BoardSize> Dot2() const;
  constexpr bool Rotate() const { return v & 1; }

 private:
  using IntWapper<BoardSize>::v;
};

template <int64_t BoardSize>
constexpr Edge<BoardSize>::Edge(const Dot<BoardSize> dot1, const Dot<BoardSize> dot2) {
  if (dot2 - dot1 == 1) {
    v = 2 * (dot1 - dot1 / (BoardSize + 1)) + 1;
  } else {
    v = 2 * dot1;
  }
  assert(Dot1() == dot1 && Dot2() == dot2);
}

template <int64_t BoardSize>
constexpr Dot<BoardSize> Edge<BoardSize>::Dot1() const {
  Int<BoardSize> dot = v >> 1;
  if (v & 1) {
    dot += dot / BoardSize;
  }
  return dot;
}

template <int64_t BoardSize>
constexpr Dot<BoardSize> Edge<BoardSize>::Dot2() const {
  Int<BoardSize> dot = v >> 1;
  if (v & 1) {
    dot += dot / BoardSize + 1;
  } else {
    dot += BoardSize + 1;
  }
  return dot;
}

template <int64_t BoardSize>
static constexpr Edge<BoardSize> InvalidEdge = -1;

}  // namespace dab::detail::model
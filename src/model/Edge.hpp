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

#include "IntWapper.hpp"
#include "Square.hpp"

namespace dab::detail::model {

class Edge : public IntWapper {
 public:
  static constexpr Int Max = 2 * BoardSize * (BoardSize + 1);

  using IntWapper::IntWapper;
  constexpr Edge(Dot dot1, Dot dot2);
  constexpr Dot Dot1() const;
  constexpr Dot Dot2() const;
  constexpr bool Rotate() const { return v & 1; }
};

constexpr Edge::Edge(Dot dot1, Dot dot2) {
  if (dot2 - dot1 == 1) {
    v = 2 * (dot1 - dot1 / (BoardSize + 1)) + 1;
  } else {
    v = 2 * dot1;
  }
  assert(Dot1() == dot1 && Dot2() == dot2);
}

constexpr Dot Edge::Dot1() const {
  Int dot = v >> 1;
  if (v & 1) {
    dot += dot / BoardSize;
  }
  return dot;
}

constexpr Dot Edge::Dot2() const {
  Int dot = v >> 1;
  if (v & 1) {
    dot += dot / BoardSize + 1;
  } else {
    dot += BoardSize + 1;
  }
  return dot;
}

static constexpr Edge InvalidEdge = -1;

}  // namespace dab::detail::model
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

#include <Dab/Common.hpp>

#include "Edge.hpp"
#include "Square.hpp"

namespace dab::detail::model {

template <int64_t BoardSize>
class NearEdgesMapper {
 public:
  constexpr NearEdgesMapper();

  static constexpr Array<Edge<BoardSize>, 4>
  GetNearEdges(const Box<BoardSize> box);

  Array<Array<Edge<BoardSize>, 4>, Box<BoardSize>::Max> BoxNearEdges;
};

template <int64_t BoardSize>
constexpr NearEdgesMapper<BoardSize>::NearEdgesMapper() {
  for (Box<BoardSize> box = 0; box < Box<BoardSize>::Max; ++box) {
    BoxNearEdges[box] = GetNearEdges(box);
  }
}

template <int64_t BoardSize>
constexpr Array<Edge<BoardSize>, 4>
NearEdgesMapper<BoardSize>::GetNearEdges(const Box<BoardSize> box) {
  Array<Edge<BoardSize>, 4> NearEdges;
  const Int<BoardSize> x = box.X();
  const Int<BoardSize> y = box.Y();
  const Dot<BoardSize> topLeft(x, y);
  const Dot<BoardSize> topRight(x + 1, y);
  const Dot<BoardSize> bottomLeft(x, y + 1);
  const Dot<BoardSize> bottomRight(x + 1, y + 1);
  NearEdges[0] = Edge(topLeft, topRight);
  NearEdges[1] = Edge(topLeft, bottomLeft);
  NearEdges[2] = Edge(bottomLeft, bottomRight);
  NearEdges[3] = Edge(topRight, bottomRight);
  return NearEdges;
}

template <int64_t BoardSize>
const Array<Edge<BoardSize>, 4>&
NearEdges(const Box<BoardSize> box) {
  static constexpr NearEdgesMapper<BoardSize> Instance;

  return Instance.BoxNearEdges[box];
}

}  // namespace dab::detail::model
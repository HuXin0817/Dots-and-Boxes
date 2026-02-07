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

#include <Dab/Model.hpp>

#include "BasicBoard.hpp"

namespace dab::detail::board {

template <int64_t BoardSize>
class EdgeCountableBoard : public BasicBoard<BoardSize> {
 public:
  EdgeCountableBoard();

  void
  Reset();
  Int<BoardSize>
  Add(const Edge<BoardSize> edge);
  Edge<BoardSize>
  FindNotContainsEdgeInBox(const Box<BoardSize> box) const;
  Edge<BoardSize>
  FindScoreableEdge() const;
  uint8_t
  EdgeCount(const Box<BoardSize> box) const;
  uint8_t
  MaxEdgeCount(const Edge<BoardSize> edge) const;

 protected:
  Array<uint8_t, Box<BoardSize>::Max> Counter;
};

template <int64_t BoardSize>
EdgeCountableBoard<BoardSize>::EdgeCountableBoard() {
  Reset();
}

template <int64_t BoardSize>
void
EdgeCountableBoard<BoardSize>::Reset() {
  BasicBoard<BoardSize>::Reset();
  Counter = Array<uint8_t, Box<BoardSize>::Max>();
}

template <int64_t BoardSize>
Int<BoardSize>
EdgeCountableBoard<BoardSize>::Add(const Edge<BoardSize> edge) {
  BasicBoard<BoardSize>::Add(edge);
  Int<BoardSize> score = 0;
  for (const Box<BoardSize> box : NearBoxes(edge)) {
    const uint8_t num = ++Counter[box];
    assert(num <= 4);
    if (num == 4) {
      ++score;
    }
  }
  return score;
}

template <int64_t BoardSize>
Edge<BoardSize>
EdgeCountableBoard<BoardSize>::FindNotContainsEdgeInBox(const Box<BoardSize> box) const {
  assert(Counter[box] == 3);
  for (const Edge<BoardSize> edge : NearEdges(box)) {
    if (BasicBoard<BoardSize>::NotContains(edge)) {
      return edge;
    }
  }
  assert(false);
  return InvalidEdge<BoardSize>;
}

template <int64_t BoardSize>
Edge<BoardSize>
EdgeCountableBoard<BoardSize>::FindScoreableEdge() const {
  for (Box<BoardSize> box = 0; box < Box<BoardSize>::Max; ++box) {
    if (Counter[box] == 3) {
      return FindNotContainsEdgeInBox(box);
    }
  }
  return InvalidEdge<BoardSize>;
}

template <int64_t BoardSize>
uint8_t
EdgeCountableBoard<BoardSize>::EdgeCount(const Box<BoardSize> box) const {
  return Counter[box];
}

template <int64_t BoardSize>
uint8_t
EdgeCountableBoard<BoardSize>::MaxEdgeCount(const Edge<BoardSize> edge) const {
  const List<Box<BoardSize>, 2>& nearBoxes = NearBoxes(edge);
  return std::max(Counter[nearBoxes.Front()], Counter[nearBoxes.Back()]);
}

}  // namespace dab::detail::board
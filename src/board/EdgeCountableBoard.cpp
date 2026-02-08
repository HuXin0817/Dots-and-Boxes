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

#include "EdgeCountableBoard.h"

namespace dab::detail::board {

void EdgeCountableBoard::Reset() {
  BasicBoard::Reset();
  Counter = Array<uint8_t, Box::Max>();
}

Int EdgeCountableBoard::Add(Edge edge) {
  BasicBoard::Add(edge);
  Int score = 0;
  for (const Box box : NearBoxes(edge)) {
    const uint8_t num = ++Counter.At(box);
    assert(num <= 4);
    if (num == 4) {
      ++score;
    }
  }
  return score;
}

Edge EdgeCountableBoard::FindNotContainsEdgeInBox(Box box) const {
  assert(Counter.At(box) == 3);
  for (const Edge edge : NearEdges(box)) {
    if (NotContains(edge)) {
      return edge;
    }
  }
  assert(false);
  return Edge::Invalid;
}

Edge EdgeCountableBoard::FindScoreableEdge() const {
  for (Box box = 0; box < Box::Max; box.Add()) {
    if (Counter.At(box) == 3) {
      return FindNotContainsEdgeInBox(box);
    }
  }
  return Edge::Invalid;
}

uint8_t EdgeCountableBoard::MaxEdgeCount(Edge edge) const {
  const List<Box, 2>& nearBoxes = NearBoxes(edge);
  return std::max(Counter.At(nearBoxes.Front()), Counter.At(nearBoxes.Back()));
}

}  // namespace dab::detail::board
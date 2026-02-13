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

#include "ScoreableEdgeBoard.h"

#include <Dab/Model.h>

namespace dab::__detail__::board {

void ScoreableEdgeBoard::Reset(const EdgeCountableBoard& newBoard) {
  EdgeCountableBoard::operator=(newBoard);
  ScoreableEdges.Clear();
}

Int ScoreableEdgeBoard::Add(Edge edge) {
  BasicBoard::Add(edge);
  Int score = 0;
  for (const Box box : edge.NearBoxes()) {
    const uint8_t count = ++Counter.At(box);
    Assert(count <= 4);
    if (count == 4) {
      ++score;
    } else if (count == 3) {
      ScoreableEdges.Append(FindNotContainsEdgeInBox(box));
    }
  }
  return score;
}

Int ScoreableEdgeBoard::MaxObtainableScore(Int endScore) {
  Int score = 0;
  while (Gaming()) {
    if (ScoreableEdges.Empty()) {
      if (const Edge edge = FindScoreableEdge(); edge != Edge::Invalid) {
        ScoreableEdges.Append(edge);
      } else {
        break;
      }
    }
    const Edge edge = ScoreableEdges.Pop();
    if (Contains(edge)) {
      continue;
    }
    const Int addScore = Add(edge);
    Assert(addScore > 0);
    score += addScore;
    if (score >= endScore) {
      break;
    }
  }
  return score;
}

}  // namespace dab::__detail__::board
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

#include "EdgeCountableBoard.hpp"

namespace dab::detail::board {

template <int64_t BoardSize>
class ScoreableEdgeBoard : public EdgeCountableBoard<BoardSize> {
 public:
  ScoreableEdgeBoard() { Reset(); }

  void Reset(const EdgeCountableBoard<BoardSize>& newBoard = EdgeCountableBoard<BoardSize>());
  Int<BoardSize> Add(Edge<BoardSize> edge);
  Int<BoardSize> MaxObtainableScore(Int<BoardSize> minScore);

 private:
  Queue<Edge<BoardSize>, Edge<BoardSize>::Max> ScoreableEdges;
};

template <int64_t BoardSize>
void ScoreableEdgeBoard<BoardSize>::Reset(const EdgeCountableBoard<BoardSize>& newBoard) {
  EdgeCountableBoard<BoardSize>::operator=(newBoard);
  ScoreableEdges.Clear();
}

template <int64_t BoardSize>
Int<BoardSize> ScoreableEdgeBoard<BoardSize>::Add(Edge<BoardSize> edge) {
  BasicBoard<BoardSize>::Add(edge);
  Int<BoardSize> score = 0;
  for (const Box<BoardSize> box : NearBoxes(edge)) {
    const uint8_t count = ++EdgeCountableBoard<BoardSize>::Counter[box];
    assert(count <= 4);
    if (count == 4) {
      ++score;
    } else if (count == 3) {
      ScoreableEdges.Append(EdgeCountableBoard<BoardSize>::FindNotContainsEdgeInBox(box));
    }
  }
  return score;
}

template <int64_t BoardSize>
Int<BoardSize> ScoreableEdgeBoard<BoardSize>::MaxObtainableScore(Int<BoardSize> endScore) {
  Int<BoardSize> score = 0;
  while (EdgeCountableBoard<BoardSize>::Gaming()) {
    if (ScoreableEdges.Empty()) {
      if (const Edge<BoardSize> edge = EdgeCountableBoard<BoardSize>::FindScoreableEdge();
          edge != InvalidEdge<BoardSize>) {
        ScoreableEdges.Append(edge);
      } else {
        break;
      }
    }
    const Edge<BoardSize> edge = ScoreableEdges.Pop();
    if (EdgeCountableBoard<BoardSize>::Contains(edge)) {
      continue;
    }
    const Int<BoardSize> addScore = Add(edge);
    assert(addScore > 0);
    score += addScore;
    if (score >= endScore) {
      break;
    }
  }
  return score;
}

}  // namespace dab::detail::board
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

#include "Robot.hpp"

namespace dab::detail::robot {

template <int64_t BoardSize>
class GreedyRobot : public Robot<BoardSize> {
 public:
  GreedyRobot() = default;

  Span<Edge<BoardSize>> BestCandidateEdges(const RelativeScoreBoard<BoardSize>& board) override;
  bool EnemyUnscoreable() const { return EnemyUnscoreableIndex < Edge<BoardSize>::Max; }
  bool Scoreable() const { return ScoreableIndex > 0; }

 protected:
  Array<Edge<BoardSize>, Edge<BoardSize>::Max>& GetEdgeBuffer() { return Edges; }

 private:
  Int<BoardSize> EnemyUnscoreableIndex;
  Int<BoardSize> ScoreableIndex;
  Array<Edge<BoardSize>, Edge<BoardSize>::Max> Edges;
};

template <int64_t BoardSize>
Span<Edge<BoardSize>> GreedyRobot<BoardSize>::BestCandidateEdges(const RelativeScoreBoard<BoardSize>& board) {
  ScoreableIndex = 0;
  EnemyUnscoreableIndex = Edge<BoardSize>::Max;

  Span<Edge<BoardSize>> emptyEdges = board.EmptyEdges();
  for (const Edge<BoardSize> edge : emptyEdges) {
    if (const uint8_t maxCount = board.MaxEdgeCount(edge); maxCount == 3) {
      Edges[ScoreableIndex++] = edge;
    } else if (maxCount < 2) {
      Edges[--EnemyUnscoreableIndex] = edge;
    }
  }
  assert(ScoreableIndex <= EnemyUnscoreableIndex);

  if (Scoreable()) {
    return Span(Edges.begin(), Edges.begin() + ScoreableIndex);
  }
  if (EnemyUnscoreable()) {
    return Span(Edges.begin() + EnemyUnscoreableIndex, Edges.end());
  }

  return Span(emptyEdges.begin(), emptyEdges.end());
}

}  // namespace dab::detail::robot
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

#include <Dab/Board.h>
#include <Dab/Model.h>

namespace dab::__detail__::robot {

class GreedyRobot {
 public:
  GreedyRobot() = default;

  bool EnemyUnscoreable() const { return EnemyUnscoreableIndex < Edge::Max; }
  bool Scoreable() const { return ScoreableIndex > 0; }
  Array<Edge, Edge::Max>& GetEdgeBuffer() { return Edges; }
  template <typename Board>
  Span<const Edge> BestCandidateEdges(const Board& board);
  template <typename Board>
  Edge SearchOne(const Board& board);

 private:
  Int EnemyUnscoreableIndex = 0;
  Int ScoreableIndex = 0;
  Array<Edge, Edge::Max> Edges;
};

template <typename Board>
Span<const Edge> GreedyRobot::BestCandidateEdges(const Board& board) {
  ScoreableIndex = 0;
  EnemyUnscoreableIndex = Edge::Max;

  Span<const Edge> emptyEdges = board.EmptyEdges();
  for (const Edge edge : emptyEdges) {
    if (const uint8_t maxCount = board.MaxEdgeCount(edge); maxCount == 3) {
      Edges.At(ScoreableIndex++) = edge;
    } else if (maxCount < 2) {
      Edges.At(--EnemyUnscoreableIndex) = edge;
    }
  }
  Assert(ScoreableIndex <= EnemyUnscoreableIndex, K(ScoreableIndex), K(EnemyUnscoreableIndex));

  if (Scoreable()) {
    return {Edges.begin(), ScoreableIndex};
  }
  if (EnemyUnscoreable()) {
    return {Edges.begin() + EnemyUnscoreableIndex, Edges.end()};
  }

  return {emptyEdges.begin(), emptyEdges.end()};
}

template <typename Board>
Edge GreedyRobot::SearchOne(const Board& board) {
  Edge result;
  for (const Edge edge : board.EmptyEdges()) {
    if (const uint8_t maxCount = board.MaxEdgeCount(edge); maxCount == 3) {
      return edge;
    } else if (maxCount < 2) {
      result = edge;
    }
  }
  return result;
}

}  // namespace dab::__detail__::robot
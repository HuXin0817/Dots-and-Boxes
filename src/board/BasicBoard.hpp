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

namespace dab::detail::board {

template <int64_t BoardSize>
class BasicBoard {
 public:
  BasicBoard();

  void
  Reset();
  void
  Add(const Edge<BoardSize> edge);
  bool
  Contains(const Edge<BoardSize> edge) const;
  bool
  NotContains(const Edge<BoardSize> edge) const;
  Span<Edge<BoardSize>>
  EmptyEdges() const;
  Span<Edge<BoardSize>>
  MoveRecord() const;
  bool
  Gaming() const;
  Int<BoardSize>
  RemainStep() const;
  Int<BoardSize>
  NowStep() const;

 private:
  Int<BoardSize> Step = 0;
  Array<Edge<BoardSize>, Edge<BoardSize>::Max> Edges;
  Array<Int<BoardSize>, Edge<BoardSize>::Max> EdgeIndexes;
};

template <int64_t BoardSize>
BasicBoard<BoardSize>::BasicBoard() {
  Reset();
}

template <int64_t BoardSize>
void
BasicBoard<BoardSize>::Reset() {
  Step = 0;
  for (Edge<BoardSize> edge = 0; edge < Edge<BoardSize>::Max; ++edge) {
    EdgeIndexes[edge] = edge;
    Edges[edge] = edge;
  }
}

template <int64_t BoardSize>
void
BasicBoard<BoardSize>::Add(const Edge<BoardSize> edge) {
  assert(NotContains(edge));
  const Edge<BoardSize> nowEdge = Edges[Step];
  const Int<BoardSize> edgeIndex = EdgeIndexes[edge];
  assert(Edges[edgeIndex] == edge);
  assert(edgeIndex >= Step);
  Edges[Step] = edge;
  Edges[edgeIndex] = nowEdge;
  EdgeIndexes[edge] = Step;
  EdgeIndexes[nowEdge] = edgeIndex;
  ++Step;
}

template <int64_t BoardSize>
bool
BasicBoard<BoardSize>::Contains(const Edge<BoardSize> edge) const {
  return EdgeIndexes[edge] < Step;
}

template <int64_t BoardSize>
bool
BasicBoard<BoardSize>::NotContains(const Edge<BoardSize> edge) const {
  return EdgeIndexes[edge] >= Step;
}

template <int64_t BoardSize>
Span<Edge<BoardSize>>
BasicBoard<BoardSize>::EmptyEdges() const {
  return Span(Edges.begin() + Step, Edges.end());
}

template <int64_t BoardSize>
Span<Edge<BoardSize>>
BasicBoard<BoardSize>::MoveRecord() const {
  return Span(Edges.begin(), Edges.begin() + Step);
}

template <int64_t BoardSize>
bool
BasicBoard<BoardSize>::Gaming() const {
  return Step < Edge<BoardSize>::Max;
}

template <int64_t BoardSize>
Int<BoardSize>
BasicBoard<BoardSize>::RemainStep() const {
  return Edge<BoardSize>::Max - Step;
}

template <int64_t BoardSize>
Int<BoardSize>
BasicBoard<BoardSize>::NowStep() const {
  return Step;
}

}  // namespace dab::detail::board
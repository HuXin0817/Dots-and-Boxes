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

class BasicBoard {
 public:
  BasicBoard() { Reset(); }

  void Reset();
  void Add(Edge edge);
  inline bool Contains(Edge edge) const { return EdgeIndexes[edge] < Step; }
  inline bool NotContains(Edge edge) const { return EdgeIndexes[edge] >= Step; }
  inline Span<Edge> EmptyEdges() const { return Span(Edges.begin() + Step, Edges.end()); }
  inline Span<Edge> MoveRecord() const { return Span(Edges.begin(), Edges.begin() + Step); }
  inline bool Gaming() const { return Step < Edge::Max; }
  inline Int RemainStep() const { return Edge::Max - Step; }
  inline Int NowStep() const { return Step; }

 private:
  Int Step = 0;
  Array<Edge, Edge::Max> Edges;
  Array<Int, Edge::Max> EdgeIndexes;
};

}  // namespace dab::detail::board
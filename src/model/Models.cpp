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

#include "Models.h"

#include <ranges>

namespace dab::__detail__::model {

constexpr Array<Edge, 4> GetNearEdges(Box box) {
  Array<Edge, 4> NearEdges;
  const Int x = box.X();
  const Int y = box.Y();
  const Dot topLeft(x, y);
  const Dot topRight(x + 1, y);
  const Dot bottomLeft(x, y + 1);
  const Dot bottomRight(x + 1, y + 1);
  NearEdges.At(0) = Edge(topLeft, topRight);
  NearEdges.At(1) = Edge(topLeft, bottomLeft);
  NearEdges.At(2) = Edge(bottomLeft, bottomRight);
  NearEdges.At(3) = Edge(topRight, bottomRight);
  return NearEdges;
}

constexpr Array<Array<Edge, 4>, Box::Max> CreateNearEdgesMapper() {
  Array<Array<Edge, 4>, Box::Max> BoxNearEdges;
  for (const Box box : std::views::iota(0, Box::Max)) {
    BoxNearEdges.At(box) = GetNearEdges(box);
  }
  return BoxNearEdges;
}

const Array<Edge, 4>& Box::NearEdges() const {
  static constexpr Array<Array<Edge, 4>, Max> Instance = CreateNearEdgesMapper();
  return Instance.At(v);
}

constexpr List<Box, 2> GetNearBoxes(Edge edge) {
  List<Box, 2> result;
  Int x = edge.Dot2().X() - 1;
  Int y = edge.Dot2().Y() - 1;
  if (x >= 0 && y >= 0) {
    result.Append(Box(x, y));
  }
  x = edge.Dot1().X();
  y = edge.Dot1().Y();
  if (x < BoardSize && y < BoardSize) {
    result.Append(Box(x, y));
  }
  return result;
}

constexpr Array<List<Box, 2>, Edge::Max> CreateNearBoxesMapper() {
  Array<List<Box, 2>, Edge::Max> EdgeNearBoxes;
  for (const Edge edge : std::views::iota(0, Edge::Max)) {
    EdgeNearBoxes.At(edge) = GetNearBoxes(edge);
  }
  return EdgeNearBoxes;
}

const List<Box, 2>& Edge::NearBoxes() const {
  static constexpr Array<List<Box, 2>, Max> Instance = CreateNearBoxesMapper();
  return Instance.At(v);
}

}  // namespace dab::__detail__::model
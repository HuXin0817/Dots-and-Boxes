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

#include <mutex>
#include <nlohmann/json.hpp>
#include <ranges>

#include "BoardSize.h"
#include "Iterable.h"
#include "Tools.h"

namespace dab {

namespace __detail__::model {

#if __BoardSize__ <= 20
#define _constexpr constexpr
#else
#define _constexpr
#endif

class IntWapper {
 public:
  constexpr IntWapper() = default;
  constexpr IntWapper(Int v) : v(v) {}
  constexpr operator Int() { return v; }
  constexpr operator Int() const { return v; }
  constexpr operator nlohmann::ordered_json() { return v; }
  constexpr operator nlohmann::ordered_json() const { return v; }

 protected:
  Int v = 0;
};

template <Int Length>
class Square : public IntWapper {
 public:
  static constexpr Int Max = Length * Length;

  constexpr Square() = default;
  constexpr Square(Int v) : IntWapper(v) { Check(); }
  constexpr Square(Int x, Int y) : IntWapper(x * Length + y) { Check(); }
  constexpr Int X() const { return v / Length; }
  constexpr Int Y() const { return v % Length; }

 private:
  constexpr void Check() const { Assert(0 <= v && v < Max, K(v), K(Max)); }
};

using Dot = Square<BoardSize + 1>;

class Edge;

class Box : public Square<BoardSize> {
 public:
  using Square::Square;
  constexpr const Array<Edge, 4>& NearEdges() const;
};

class Edge : public IntWapper {
 public:
  static constexpr Int Max = 2 * BoardSize * (BoardSize + 1);
  static constexpr Int Invalid = -1;

  constexpr Edge() : IntWapper(Invalid) {}
  constexpr Edge(Int v) : IntWapper(v) { Check(); }
  constexpr Edge(Dot dot1, Dot dot2);
  constexpr void Reset() { v = Invalid; }
  constexpr Dot Dot1() const;
  constexpr Dot Dot2() const;
  constexpr bool Rotate() const;
  constexpr bool Valid() const { return v != Invalid; }
  constexpr const List<Box, 2>& NearBoxes() const;

 private:
  constexpr void Check() const { Assert(Invalid <= v && v < Max, K(v), K(Max)); }
};

constexpr Edge::Edge(Dot dot1, Dot dot2) {
  Assert(dot1 < dot2, K(dot1), K(dot2));
  Dot dif = dot2 - dot1;
  Assert(dif == 1 || dif == BoardSize + 1, K(dot1), K(dot2), K(dif), K(BoardSize + 1));
  if (dif == 1) {
    v = 2 * (dot1 - dot1 / (BoardSize + 1)) + 1;
  } else {
    v = 2 * dot1;
  }
  Assert(Dot1() == dot1 && Dot2() == dot2, K(Dot1()), K(dot1), K(Dot2()), K(dot2));
}

constexpr Dot Edge::Dot1() const {
  Assert(Valid(), K(v));
  Int dot = v >> 1;
  if (v & 1) {
    dot += dot / BoardSize;
  }
  return dot;
}

constexpr Dot Edge::Dot2() const {
  Assert(Valid(), K(v));
  Int dot = v >> 1;
  if (v & 1) {
    dot += dot / BoardSize + 1;
  } else {
    dot += BoardSize + 1;
  }
  return dot;
}

constexpr bool Edge::Rotate() const {
  Assert(Valid(), K(v));
  return v & 1;
}

template <typename T>
constexpr auto Iota() {
  return std::views::iota(static_cast<Int>(0), T::Max);
};

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
  for (const Box box : Iota<Box>()) {
    BoxNearEdges.At(box) = GetNearEdges(box);
  }
  return BoxNearEdges;
}

constexpr const Array<Edge, 4>& Box::NearEdges() const {
  static _constexpr Array<Array<Edge, 4>, Max> Instance = CreateNearEdgesMapper();
  if constexpr (DebugMode) {
    static std::once_flag once;
    std::call_once(once, [&]() -> void { LogDebug({{"NearEdgesMapper", Instance}}); });
  }
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
  for (const Edge edge : Iota<Edge>()) {
    EdgeNearBoxes.At(edge) = GetNearBoxes(edge);
  }
  return EdgeNearBoxes;
}

constexpr const List<Box, 2>& Edge::NearBoxes() const {
  static _constexpr Array<List<Box, 2>, Max> Instance = CreateNearBoxesMapper();
  if constexpr (DebugMode) {
    static std::once_flag once;
    std::call_once(once, [&]() -> void { LogDebug({{"NearBoxesMapper", Instance}}); });
  }
  return Instance.At(v);
}

}  // namespace __detail__::model

using __detail__::model::Box;
using __detail__::model::Dot;
using __detail__::model::Edge;
using __detail__::model::Iota;

}  // namespace dab
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

#include <nlohmann/json.hpp>
#include <ranges>

#include "Common.h"
#include "Iterable.h"
#include "Logging.h"

namespace dab {

template <typename T>
static constexpr auto Iota() {
  return std::views::iota(static_cast<decltype(T::Max)>(0), T::Max);
}

namespace __detail__ {

namespace model {

class IntWrapper {
 public:
  constexpr IntWrapper() = default;
  constexpr IntWrapper(Int v) : v(v) {}
  constexpr operator Int() const { return v; }
  constexpr operator nlohmann::ordered_json() const { return v; }

 protected:
  Int v = 0;
};

template <Int Length>
class Square : public IntWrapper {
 public:
  static constexpr Int Max = Length * Length;

  using IntWrapper::IntWrapper;
  constexpr Square(Int x, Int y) : IntWrapper(x * Length + y) { Assert(x < Max && y < Max, K(x), K(y)); }
  constexpr Int X() const { return v / Length; }
  constexpr Int Y() const { return v % Length; }
};

using Dot = Square<BoardSize + 1>;

class Edge;

class Box : public Square<BoardSize> {
 public:
  using Square::Square;
  constexpr const Array<Edge, 4>& NearEdges() const;

 private:
  constexpr Array<Edge, 4> GetNearEdges() const;
};

class Edge : public IntWrapper {
 public:
  static constexpr Int Max = 2 * BoardSize * (BoardSize + 1);
  static constexpr Int Invalid = -1;

  using IntWrapper::IntWrapper;
  constexpr Edge(Dot dot1, Dot dot2);
  constexpr Dot Dot1() const;
  constexpr Dot Dot2() const;
  constexpr bool Rotate() const { return v & 1; }
  constexpr const List<Box, 2>& NearBoxes() const;

 private:
  constexpr List<Box, 2> GetNearBoxes() const;
};

constexpr Edge::Edge(Dot dot1, Dot dot2) {
  Assert(dot1 < dot2, K(dot1), K(dot2));
  if (dot2 - dot1 == 1) {
    v = 2 * (dot1 - dot1 / (BoardSize + 1)) + 1;
  } else {
    v = 2 * dot1;
  }
  Assert(Dot1() == dot1 && Dot2() == dot2, K(Dot1()), K(dot1), K(Dot2()), K(dot2));
}

constexpr Dot Edge::Dot1() const {
  Int dot = v >> 1;
  if (v & 1) {
    dot += dot / BoardSize;
  }
  return dot;
}

constexpr Dot Edge::Dot2() const {
  Int dot = v >> 1;
  if (v & 1) {
    dot += dot / BoardSize + 1;
  } else {
    dot += BoardSize + 1;
  }
  return dot;
}

constexpr Array<Edge, 4> Box::GetNearEdges() const {
  List<Edge, 4> NearEdges;
  const Int x = X();
  const Int y = Y();
  const Dot topLeft(x, y);
  const Dot topRight(x + 1, y);
  const Dot bottomLeft(x, y + 1);
  const Dot bottomRight(x + 1, y + 1);
  NearEdges.Append({topLeft, topRight});
  NearEdges.Append({topLeft, bottomLeft});
  NearEdges.Append({bottomLeft, bottomRight});
  NearEdges.Append({topRight, bottomRight});
  std::ranges::sort(NearEdges);
  return NearEdges;
}

constexpr const Array<Edge, 4>& Box::NearEdges() const {
  static constexpr Array<Array<Edge, 4>, Max> Instance = []() -> Array<Array<Edge, 4>, Max> {
    Array<Array<Edge, 4>, Max> Mapper;
    for (const Box box : Iota<Box>()) {
      Mapper.At(box) = box.GetNearEdges();
    }
    return Mapper;
  }();
  return Instance.At(v);
}

constexpr List<Box, 2> Edge::GetNearBoxes() const {
  List<Box, 2> result;
  Int x = Dot2().X() - 1;
  Int y = Dot2().Y() - 1;
  if (x >= 0 && y >= 0) {
    result.Append({x, y});
  }
  x = Dot1().X();
  y = Dot1().Y();
  if (x < BoardSize && y < BoardSize) {
    result.Append({x, y});
  }
  return result;
}

constexpr const List<Box, 2>& Edge::NearBoxes() const {
  static constexpr Array<List<Box, 2>, Max> Instance = []() -> Array<List<Box, 2>, Max> {
    Array<List<Box, 2>, Max> Mapper;
    for (const Edge edge : Iota<Edge>()) {
      Mapper.At(edge) = edge.GetNearBoxes();
    }
    return Mapper;
  }();
  return Instance.At(v);
}

}  // namespace model

using model::Box;
using model::Dot;
using model::Edge;

}  // namespace __detail__

}  // namespace dab
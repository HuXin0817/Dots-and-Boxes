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

#include <Dab/BoardSize.h>
#include <Dab/Common.h>
#include <Dab/Tools.h>

#include <ranges>

namespace dab {

namespace __detail__::model {

class IntWapper {
 public:
  constexpr IntWapper() = default;
  constexpr IntWapper(Int v) : v(v) {}
  constexpr operator Int() { return v; }
  constexpr operator Int() const { return v; }

 protected:
  Int v = 0;
};

template <Int Length>
class Square : public IntWapper {
 public:
  static constexpr Int Max = Length * Length;

  using IntWapper::IntWapper;
  constexpr Square(Int x, Int y) : IntWapper(x * Length + y) {}
  constexpr Int X() const { return v / Length; }
  constexpr Int Y() const { return v % Length; }
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

  using IntWapper::IntWapper;
  constexpr Edge() { Reset(); }
  constexpr Edge(Dot dot1, Dot dot2);
  constexpr void Reset() { v = Invalid; }
  constexpr Dot Dot1() const;
  constexpr Dot Dot2() const;
  constexpr bool Rotate() const;
  constexpr bool Valid() const { return v != Invalid; }
  constexpr const List<Box, 2>& NearBoxes() const;
};

constexpr Edge::Edge(Dot dot1, Dot dot2) {
  if (dot2 - dot1 == 1) {
    v = 2 * (dot1 - dot1 / (BoardSize + 1)) + 1;
  } else {
    v = 2 * dot1;
  }
  Assert(Dot1() == dot1 && Dot2() == dot2);
}

constexpr Dot Edge::Dot1() const {
  Assert(Valid());
  Int dot = v >> 1;
  if (v & 1) {
    dot += dot / BoardSize;
  }
  return dot;
}

constexpr Dot Edge::Dot2() const {
  Assert(Valid());
  Int dot = v >> 1;
  if (v & 1) {
    dot += dot / BoardSize + 1;
  } else {
    dot += BoardSize + 1;
  }
  return dot;
}

constexpr bool Edge::Rotate() const {
  Assert(Valid());
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
  if constexpr (BoardSize < 36) {
    static constexpr Array<Array<Edge, 4>, Max> Instance = CreateNearEdgesMapper();
    return Instance.At(v);
  } else {
    static Array<Array<Edge, 4>, Max> Instance = CreateNearEdgesMapper();
    return Instance.At(v);
  }
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
  if constexpr (BoardSize < 36) {
    static constexpr Array<List<Box, 2>, Max> Instance = CreateNearBoxesMapper();
    return Instance.At(v);
  } else {
    static Array<List<Box, 2>, Max> Instance = CreateNearBoxesMapper();
    return Instance.At(v);
  }
}

class Turn : public IntWapper {
 public:
  constexpr Turn() { Reset(); }
  constexpr void Reset() { v = Player1Turn; }
  constexpr void Add() { v = -v; }
  constexpr bool IsPlayer1Turn() const { return v == Player1Turn; }
  constexpr bool IsPlayer2Turn() const { return v == Player2Turn; }

 private:
  static constexpr Int Player1Turn = 1;
  static constexpr Int Player2Turn = -Player1Turn;
};

class ScoreMap {
 public:
  ScoreMap() = default;

  void Reset();
  void Add(Edge edge, Int score);
  void Add(const ScoreMap& other);
  Span<const Edge> Export(List<Edge, Edge::Max>& edges);

 private:
  Array<int, Edge::Max> Time;
  Array<int, Edge::Max> Score;
};

inline void ScoreMap::Reset() {
  std::ranges::fill(Time, 0);
  std::ranges::fill(Score, 0);
}

inline void ScoreMap::Add(Edge edge, Int score) {
  ++Time.At(edge);
  Score.At(edge) += score;
}

inline void ScoreMap::Add(const ScoreMap& other) {
  for (const Int i : Iota<Edge>()) {
    Time.At(i) += other.Time.At(i);
    Score.At(i) += other.Score.At(i);
  }
}

inline Span<const Edge> ScoreMap::Export(List<Edge, Edge::Max>& edges) {
  edges.Clear();
  float maxScore = 0.0;
  for (const Edge edge : Iota<Edge>()) {
    if (Time.At(edge) > 0) {
      if (const float score = static_cast<float>(Score.At(edge)) / static_cast<float>(Time.At(edge));
          score > maxScore || edges.Empty()) {
        maxScore = score;
        edges.ClearAndSet(edge);
      } else if (score == maxScore) {
        edges.Append(edge);
      }
    }
  }
  return {edges.begin(), edges.Size()};
}

}  // namespace __detail__::model

using __detail__::model::Box;
using __detail__::model::Dot;
using __detail__::model::Edge;
using __detail__::model::Iota;
using __detail__::model::ScoreMap;
using __detail__::model::Turn;

}  // namespace dab
#pragma once

#include <cassert>
#include <ranges>

#include "Common.h"
#include "Iterable.h"

#if __BoardSize__ > 30
#define __constexpr__
#else
#define __constexpr__ constexpr
#endif

namespace dab::__detail__::model {

template <typename T>
constexpr auto Iota = std::views::iota(static_cast<decltype(T::Max)>(0), T::Max);

class IntWrapper {
 public:
  constexpr IntWrapper() = default;
  constexpr IntWrapper(Int value) : Value(value) {}
  constexpr operator Int() const { return Value; }

 protected:
  Int Value = 0;
};

template <Int Length>
  requires(BoardSize <= Length && Length <= BoardSize + 1)
class Square : public IntWrapper {
 public:
  static constexpr Int Max = Length * Length;

  using IntWrapper::IntWrapper;
  constexpr Square(Int x, Int y) : IntWrapper(x * Length + y) { assert(x < Max && y < Max); }
  constexpr Int X() const { return Value / Length; }
  constexpr Int Y() const { return Value % Length; }
};

using Dot = Square<BoardSize + 1>;

class Edge;

class Box : public Square<BoardSize> {
 public:
  using Square::Square;
  constexpr const iterable::Array<Edge, 4>& NearEdges() const;

 private:
  constexpr iterable::Array<Edge, 4> GetNearEdges() const;
};

class Edge : public IntWrapper {
 public:
  static constexpr Int Max = 2 * BoardSize * (BoardSize + 1);
  static constexpr Int Invalid = -1;

  using IntWrapper::IntWrapper;
  constexpr Edge(Dot dot1, Dot dot2);
  constexpr Dot Dot1() const;
  constexpr Dot Dot2() const;
  constexpr bool Rotate() const { return Value & 1; }
  constexpr const iterable::List<Box, 2>& NearBoxes() const;

 private:
  constexpr iterable::List<Box, 2> GetNearBoxes() const;
};

constexpr Edge::Edge(Dot dot1, Dot dot2) {
  assert(dot1 < dot2);
  if (dot2 - dot1 == 1) {
    Value = 2 * (dot1 - dot1 / (BoardSize + 1)) + 1;
  } else {
    Value = 2 * dot1;
  }
  assert(Dot1() == dot1 && Dot2() == dot2);
}

constexpr Dot Edge::Dot1() const {
  Int dot = Value >> 1;
  if (Value & 1) {
    dot += dot / BoardSize;
  }
  return dot;
}

constexpr Dot Edge::Dot2() const {
  Int dot = Value >> 1;
  if (Value & 1) {
    dot += dot / BoardSize + 1;
  } else {
    dot += BoardSize + 1;
  }
  return dot;
}

constexpr iterable::Array<Edge, 4> Box::GetNearEdges() const {
  iterable::List<Edge, 4> nearEdges;
  const Int x = X();
  const Int y = Y();
  const Dot topLeft(x, y);
  const Dot topRight(x + 1, y);
  const Dot bottomLeft(x, y + 1);
  const Dot bottomRight(x + 1, y + 1);
  nearEdges.Append({topLeft, topRight});
  nearEdges.Append({topLeft, bottomLeft});
  nearEdges.Append({bottomLeft, bottomRight});
  nearEdges.Append({topRight, bottomRight});
  std::ranges::sort(nearEdges);
  return nearEdges;
}

constexpr const iterable::Array<Edge, 4>& Box::NearEdges() const {
  static __constexpr__ iterable::Array<iterable::Array<Edge, 4>, Max> Mapper =
      []() -> iterable::Array<iterable::Array<Edge, 4>, Max> {
    iterable::Array<iterable::Array<Edge, 4>, Max> mapper;
    for (const Box box : Iota<Box>) {
      mapper.At(box) = box.GetNearEdges();
    }
    return mapper;
  }();
  return Mapper.At(Value);
}

constexpr iterable::List<Box, 2> Edge::GetNearBoxes() const {
  iterable::List<Box, 2> result;
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

constexpr const iterable::List<Box, 2>& Edge::NearBoxes() const {
  static __constexpr__ iterable::Array<iterable::List<Box, 2>, Max> Mapper =
      []() -> iterable::Array<iterable::List<Box, 2>, Max> {
    iterable::Array<iterable::List<Box, 2>, Max> mapper;
    for (const Edge edge : Iota<Edge>) {
      mapper.At(edge) = edge.GetNearBoxes();
    }
    return mapper;
  }();
  return Mapper.At(Value);
}

}  // namespace dab::__detail__::model
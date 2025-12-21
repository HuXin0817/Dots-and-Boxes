#pragma once

#include <sstream>
#include <string>

#include "../common/Macro.hpp"

template <int A>
class Square {
  public:
  Square(int x, int y) : v(x * Size + y) {
  }

  int
  X() const {
    return v / Size;
  }

  int
  Y() const {
    return v % Size;
  }

  operator std::string() const {
    std::stringstream ss;
    ss << "(" << X() << ", " << Y() << ")";
    return ss.str();
  }

  static constexpr int Size = A;
  static constexpr int Max = Size * Size;

  V(Square<A>)
};

#define DefSquareSubClass(SubClass, SquareClass) \
  template <int BoardSize>                       \
  class SubClass : public SquareClass {          \
public:                                          \
    SubClass() : SquareClass() {                 \
    }                                            \
    SubClass(int v) : SquareClass(v) {           \
    }                                            \
    SubClass(int x, int y) : SquareClass(x, y) { \
    }                                            \
    operator int() {                             \
      return SquareClass::v;                     \
    }                                            \
  }

DefSquareSubClass(Dot, Square<BoardSize + 1>);
DefSquareSubClass(Box, Square<BoardSize>);

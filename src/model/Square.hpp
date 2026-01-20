#pragma once

#include <sstream>
#include <string>

#include "../common/Macro.hpp"

namespace dab::model {

#define DefSquareClass(Class, SizeExpr)               \
  template <int BoardSize>                            \
  class Class {                                       \
public:                                               \
    constexpr Class(int x, int y) : v(x * Size + y) { \
    }                                                 \
    constexpr int                                     \
    X() const {                                       \
      return v / Size;                                \
    }                                                 \
    constexpr int                                     \
    Y() const {                                       \
      return v % Size;                                \
    }                                                 \
    explicit                                          \
    operator std::string() const {                    \
      std::stringstream ss;                           \
      ss << "(" << X() << ", " << Y() << ")";         \
      return ss.str();                                \
    }                                                 \
    static constexpr int Size = SizeExpr;             \
    static constexpr int Max = Size * Size;           \
    IntValueObject(Class)                             \
  }

DefSquareClass(Dot, BoardSize + 1);
DefSquareClass(Box, BoardSize);

}  // namespace dab::model

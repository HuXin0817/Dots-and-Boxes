#pragma once

#include "../common/Macro.hpp"

namespace dab::model {

#define DefSquareClass(Class, SizeExpr)               \
  template <int BoardSize, typename SizeType>         \
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
    static constexpr int Size = SizeExpr;             \
    static constexpr int Max = Size * Size;           \
    IntValueObject(Class)                             \
  }

DefSquareClass(Dot, BoardSize + 1);
DefSquareClass(Box, BoardSize);

}  // namespace dab::model

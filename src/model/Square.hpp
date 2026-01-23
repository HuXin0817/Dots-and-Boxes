#pragma once

#include "../common/Macro.hpp"

#define DefSquareClass(Class, SizeExpr)                                               \
  template <int64_t BoardSize>                                                        \
  class Class {                                                                       \
public:                                                                               \
    constexpr Class(SizeType<BoardSize> x, SizeType<BoardSize> y) : v(x * Size + y) { \
    }                                                                                 \
    constexpr SizeType<BoardSize>                                                     \
    X() const {                                                                       \
      return v / Size;                                                                \
    }                                                                                 \
    constexpr SizeType<BoardSize>                                                     \
    Y() const {                                                                       \
      return v % Size;                                                                \
    }                                                                                 \
    static constexpr SizeType<BoardSize> Size = SizeExpr;                             \
    static constexpr SizeType<BoardSize> Max = Size * Size;                           \
    IntValueObject(Class, SizeType<BoardSize>)                                        \
  }

DefSquareClass(Dot, BoardSize + 1);
DefSquareClass(Box, BoardSize);

#pragma once

#include "../common/Macro.hpp"

#define DefSquareClass(Class, SizeExpr)                         \
  template <int64_t BoardSize>                                  \
  class Class {                                                 \
public:                                                         \
    constexpr Class(SizeType x, SizeType y) : v(x * Size + y) { \
    }                                                           \
    constexpr SizeType                                          \
    X() const {                                                 \
      return v / Size;                                          \
    }                                                           \
    constexpr SizeType                                          \
    Y() const {                                                 \
      return v % Size;                                          \
    }                                                           \
    static constexpr SizeType Size = SizeExpr;                  \
    static constexpr SizeType Max = Size * Size;                \
    IntValueObject(Class)                                       \
  }

DefSquareClass(Dot, BoardSize + 1);
DefSquareClass(Box, BoardSize);

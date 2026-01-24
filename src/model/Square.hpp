#pragma once

#include "SizeType.hpp"

template <int64_t BoardSize, int64_t Length>
class Square {
  public:
  static constexpr Int<BoardSize> Size = Length;
  static constexpr Int<BoardSize> Max = Size * Size;

  Square() = default;

  Square(Int<BoardSize> v) : v(v) {
  }

  Square(Int<BoardSize> x, Int<BoardSize> y) : v(x * Size + y) {
  }

  Int<BoardSize>
  Value() const {
    return v;
  }

  Int<BoardSize>
  X() const {
    return v / Size;
  }

  Int<BoardSize>
  Y() const {
    return v % Size;
  }

  private:
  Int<BoardSize> v;
};

template <int64_t BoardSize>
using Box = Square<BoardSize, BoardSize>;

template <int64_t BoardSize>
using Dot = Square<BoardSize, BoardSize + 1>;

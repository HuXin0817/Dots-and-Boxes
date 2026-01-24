#pragma once

#include "SizeType.hpp"

template <int64_t BoardSize, int64_t Length>
class Square {
  public:
  static constexpr SizeType<BoardSize> Size = Length;
  static constexpr SizeType<BoardSize> Max = Size * Size;

  Square() = default;

  Square(SizeType<BoardSize> v) : v(v) {
  }

  Square(SizeType<BoardSize> x, SizeType<BoardSize> y) : v(x * Size + y) {
  }

  SizeType<BoardSize>
  Value() const {
    return v;
  }

  SizeType<BoardSize>
  X() const {
    return v / Size;
  }

  SizeType<BoardSize>
  Y() const {
    return v % Size;
  }

  private:
  SizeType<BoardSize> v;
};

template <int64_t BoardSize>
using Box = Square<BoardSize, BoardSize>;

template <int64_t BoardSize>
using Dot = Square<BoardSize, BoardSize + 1>;

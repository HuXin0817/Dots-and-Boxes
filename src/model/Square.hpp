#pragma once

#include "SizeType.hpp"

namespace dab {

template <int64_t BoardSize, int64_t Length>
class Square {
  public:
  static constexpr SizeType<BoardSize> Size = Length;
  static constexpr SizeType<BoardSize> Max = Size * Size;

  Square() = default;
  Square(SizeType<BoardSize> v);
  Square(SizeType<BoardSize> x, SizeType<BoardSize> y);

  operator SizeType<BoardSize>() const;
  SizeType<BoardSize>
  X() const;
  SizeType<BoardSize>
  Y() const;

  private:
  SizeType<BoardSize> v;
};

template <int64_t BoardSize, int64_t Length>
Square<BoardSize, Length>::Square(SizeType<BoardSize> v) : v(v) {
}

template <int64_t BoardSize, int64_t Length>
Square<BoardSize, Length>::Square(SizeType<BoardSize> x, SizeType<BoardSize> y) : v(x * Size + y) {
}

template <int64_t BoardSize, int64_t Length>
Square<BoardSize, Length>::
operator SizeType<BoardSize>() const {
  return v;
}

template <int64_t BoardSize, int64_t Length>
SizeType<BoardSize>
Square<BoardSize, Length>::X() const {
  return v / Size;
}

template <int64_t BoardSize, int64_t Length>
SizeType<BoardSize>
Square<BoardSize, Length>::Y() const {
  return v % Size;
}

template <int64_t BoardSize>
using Box = Square<BoardSize, BoardSize>;

template <int64_t BoardSize>
using Dot = Square<BoardSize, BoardSize + 1>;

}  // namespace dab

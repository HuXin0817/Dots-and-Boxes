#pragma once

#include "Types.hpp"

namespace dab {

template <int64_t BoardSize, int64_t Length>
class Square {
  public:
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
Square<BoardSize, Length>::Square(SizeType<BoardSize> x, SizeType<BoardSize> y) : v(x * Length + y) {
}

template <int64_t BoardSize, int64_t Length>
Square<BoardSize, Length>::
operator SizeType<BoardSize>() const {
  return v;
}

template <int64_t BoardSize, int64_t Length>
SizeType<BoardSize>
Square<BoardSize, Length>::X() const {
  return v / Length;
}

template <int64_t BoardSize, int64_t Length>
SizeType<BoardSize>
Square<BoardSize, Length>::Y() const {
  return v % Length;
}

template <int64_t BoardSize, int64_t Length>
class Limits<Square<BoardSize, Length>> {
  public:
  static constexpr SizeType<BoardSize> Max = Length * Length;
};

template <int64_t BoardSize>
using Box = Square<BoardSize, BoardSize>;

template <int64_t BoardSize>
using Dot = Square<BoardSize, BoardSize + 1>;

}  // namespace dab

#pragma once

#include "Types.hpp"

namespace dab {

template <int64_t BoardSize, int64_t Length>
class Square : public SizeTypeWapper<BoardSize> {
  public:
  using SizeTypeWapper<BoardSize>::SizeTypeWapper;
  constexpr Square(SizeType<BoardSize> x, SizeType<BoardSize> y);

  constexpr SizeType<BoardSize>
  X() const;
  constexpr SizeType<BoardSize>
  Y() const;
};

template <int64_t BoardSize, int64_t Length>
constexpr Square<BoardSize, Length>::Square(SizeType<BoardSize> x, SizeType<BoardSize> y)
    : SizeTypeWapper<BoardSize>(x * Length + y) {
}

template <int64_t BoardSize, int64_t Length>
constexpr SizeType<BoardSize>
Square<BoardSize, Length>::X() const {
  return SizeTypeWapper<BoardSize>::v / Length;
}

template <int64_t BoardSize, int64_t Length>
constexpr SizeType<BoardSize>
Square<BoardSize, Length>::Y() const {
  return SizeTypeWapper<BoardSize>::v % Length;
}

template <int64_t BoardSize, int64_t Length>
class Limits<Square<BoardSize, Length>> {
  public:
  static constexpr Square<BoardSize, Length> Max = Length * Length;
};

template <int64_t BoardSize>
using Box = Square<BoardSize, BoardSize>;

template <int64_t BoardSize>
using Dot = Square<BoardSize, BoardSize + 1>;

}  // namespace dab

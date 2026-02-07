#pragma once

#include "Types.hpp"

namespace dab::detail::model {

template <int64_t BoardSize, int64_t Length>
class Square : public IntWapper<BoardSize> {
 public:
  using IntWapper<BoardSize>::IntWapper;
  constexpr Square(Int<BoardSize> x, Int<BoardSize> y);

  static constexpr Int<BoardSize> Max = Length * Length;

  constexpr Int<BoardSize>
  X() const;
  constexpr Int<BoardSize>
  Y() const;
};

template <int64_t BoardSize, int64_t Length>
constexpr Square<BoardSize, Length>::Square(Int<BoardSize> x, Int<BoardSize> y) : IntWapper<BoardSize>(x * Length + y) {
}

template <int64_t BoardSize, int64_t Length>
constexpr Int<BoardSize>
Square<BoardSize, Length>::X() const {
  return IntWapper<BoardSize>::v / Length;
}

template <int64_t BoardSize, int64_t Length>
constexpr Int<BoardSize>
Square<BoardSize, Length>::Y() const {
  return IntWapper<BoardSize>::v % Length;
}

template <int64_t BoardSize>
using Box = Square<BoardSize, BoardSize>;

template <int64_t BoardSize>
using Dot = Square<BoardSize, BoardSize + 1>;

}  // namespace dab::detail::model
#pragma once

#include "Types.hpp"

namespace dab::detail::model {

template <int64_t BoardSize>
class Turn : public IntWapper<BoardSize> {
  public:
  constexpr Turn();

  constexpr void Reset();
  constexpr void Add();
  constexpr bool IsPlayer1Turn() const;
  constexpr bool IsPlayer2Turn() const;

  private:
  static constexpr Int<BoardSize> Player1Turn = 1;
  static constexpr Int<BoardSize> Player2Turn = -Player1Turn;
};

template <int64_t BoardSize>
constexpr Turn<BoardSize>::Turn() : IntWapper<BoardSize>(1) {}

template <int64_t BoardSize>
constexpr void Turn<BoardSize>::Reset() {
  IntWapper<BoardSize>::v = Player1Turn;
}

template <int64_t BoardSize>
constexpr void Turn<BoardSize>::Add() {
  IntWapper<BoardSize>::v = -IntWapper<BoardSize>::v;
}

template <int64_t BoardSize>
constexpr bool Turn<BoardSize>::IsPlayer1Turn() const {
  return IntWapper<BoardSize>::v == Player1Turn;
}

template <int64_t BoardSize>
constexpr bool Turn<BoardSize>::IsPlayer2Turn() const {
  return IntWapper<BoardSize>::v == Player2Turn;
}

}  // namespace dab::detail::model
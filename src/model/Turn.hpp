#pragma once

namespace dab::detail::model {

class Turn {
  public:
  constexpr Turn();

  constexpr void
  Reset();
  constexpr void
  Add();
  constexpr bool
  IsPlayer1Turn() const;
  constexpr bool
  IsPlayer2Turn() const;

  private:
  bool v;

  static constexpr bool Player1 = true;
};

inline constexpr Turn::Turn() : v(Player1) {
}

inline constexpr void
Turn::Reset() {
  v = Player1;
}

inline constexpr void
Turn::Add() {
  v = !v;
}

inline constexpr bool
Turn::IsPlayer1Turn() const {
  return v;
}

inline constexpr bool
Turn::IsPlayer2Turn() const {
  return !v;
}

}  // namespace dab::detail::model
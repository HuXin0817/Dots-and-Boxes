#pragma once

namespace dab {

class Turn {
  public:
  Turn();

  void
  Reset();
  void
  Add();
  bool
  IsPlayer1Turn() const;
  bool
  IsPlayer2Turn() const;

  private:
  bool v;

  static constexpr bool Player1 = true;
};

inline Turn::Turn() {
  Reset();
}

inline void
Turn::Reset() {
  v = Player1;
}

inline void
Turn::Add() {
  v = !v;
}

inline bool
Turn::IsPlayer1Turn() const {
  return v;
}

inline bool
Turn::IsPlayer2Turn() const {
  return !v;
}

}  // namespace dab

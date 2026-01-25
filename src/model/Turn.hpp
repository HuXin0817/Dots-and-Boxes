#pragma once

class Turn {
  public:
  Turn();

  void
  Reset();
  void
  Change();
  bool
  IsPlayer1() const;
  bool
  IsPlayer2() const;

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
Turn::Change() {
  v = !v;
}

inline bool
Turn::IsPlayer1() const {
  return v;
}

inline bool
Turn::IsPlayer2() const {
  return !v;
}

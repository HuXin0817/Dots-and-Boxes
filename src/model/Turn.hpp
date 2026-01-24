#pragma once

class Turn {
  public:
  Turn() = default;
  Turn(const bool v);

  void
  Change();
  bool
  Value() const;

  private:
  bool v = true;
};

inline Turn::Turn(const bool v) : v(v) {
}

inline void
Turn::Change() {
  v = !v;
}

inline bool
Turn::Value() const {
  return v;
}

inline Turn Player1Turn = true;
inline Turn Player2Turn = !Player1Turn.Value();

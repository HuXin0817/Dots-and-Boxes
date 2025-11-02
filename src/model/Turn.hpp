#pragma once

class PlayerTurn {
  public:
  PlayerTurn() = default;

  PlayerTurn(bool v) : v(v) {
  }

  void
  Change() {
    v = !v;
  }

  operator bool() const {
    return v;
  }

  private:
  bool v = true;
};

static const PlayerTurn Player1Turn = true;
static const PlayerTurn Player2Turn = !Player1Turn;

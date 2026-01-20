#pragma once

namespace dab::model {

class PlayerTurn {
  public:
  PlayerTurn() = default;

  PlayerTurn(const bool v) : v(v) {
  }

  void
  Change() {
    v = !v;
  }

  bool
  Value() const {
    return v;
  }

  private:
  bool v = true;
};

static const PlayerTurn Player1Turn = true;
static const PlayerTurn Player2Turn = !Player1Turn.Value();

}  // namespace dab::model

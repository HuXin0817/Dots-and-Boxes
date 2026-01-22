#pragma once

namespace dab::model {

class Turn {
  public:
  Turn() = default;

  Turn(const bool v) : v(v) {
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

static const Turn Player1Turn = true;
static const Turn Player2Turn = !Player1Turn.Value();

}  // namespace dab::model

#pragma once

#include "../Board.h"
#include "../PlayerType.h"

namespace dab::__detail__::robot {

class Robot {
 public:
  virtual ~Robot() = default;

  virtual model::Edge Move(const board::GameBoard& board) = 0;

  static std::unique_ptr<Robot> Create(PlayerType playerType);
};

}  // namespace dab::__detail__::robot
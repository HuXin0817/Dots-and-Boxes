#include "Robot.h"

using namespace dab;
using namespace dab::__detail__;

static constexpr PlayerType Robot1Type = dab::DefaultPlayerType;
static constexpr PlayerType Robot2Type = dab::DefaultPlayerType;

int main() {
  std::unique_ptr<robot::Robot> robot1 = robot::Robot::Create(Robot1Type);
  std::unique_ptr<robot::Robot> robot2 = robot::Robot::Create(Robot2Type);

  board::GameBoard board;
  while (board.Gaming()) {
    if (board.IsPlayer1Turn()) {
      board.Add(robot1->Move(board));
    } else {
      board.Add(robot2->Move(board));
    }
  }

  return 0;
}
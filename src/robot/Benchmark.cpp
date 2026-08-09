#include "Robot.h"

using namespace dab;
using namespace dab::__detail__::robot;
using namespace dab::__detail__::board;

int main() {
  std::unique_ptr<Robot> robot1 = Robot::Create(DefaultPlayerType);
  std::unique_ptr<Robot> robot2 = Robot::Create(DefaultPlayerType);

  while (true) {
    GameBoard board;

    while (board.Gaming()) {
      if (board.IsPlayer1Turn()) {
        board.Add(robot1->Move(board));
      } else {
        board.Add(robot2->Move(board));
      }
    }
  }

  return 0;
}
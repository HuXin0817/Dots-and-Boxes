/*
Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2025 Xin Hu <202219120810@stu.cdut.edu.cn>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#pragma once

#include <Dab/Board.h>
#include <Dab/PlayerType.h>

#include "robot/GreedyRobot.h"
#include "robot/ImproveGreedyRobot.h"
#include "robot/MonteCarloRobot.h"
#include "robot/ParallelSearchRobot.h"
#include "robot/SimulationRobot.h"

namespace dab {

namespace __detail__::robot {

class Robot {
 public:
  virtual ~Robot() = default;

  virtual Span<const Edge> BestCandidateEdges(const LoggingBoard& board) = 0;

  static std::unique_ptr<Robot> Create(PlayerType playerType);
};

template <typename Derived>
class RobotWapper : public Robot, public Derived {
 public:
  using Derived::Derived;

  template <typename Board>
  Span<const Edge> BestCandidateEdges(const Board& board) {
    return Derived::template BestCandidateEdges<>(board);
  }
  Span<const Edge> BestCandidateEdges(const LoggingBoard& board) override { return BestCandidateEdges<>(board); }

  ~RobotWapper() override = default;
};

inline std::unique_ptr<Robot> Robot::Create(PlayerType playerType) {
  switch (playerType) {
    case PlayerType::GreedyRobot:
      return std::make_unique<RobotWapper<GreedyRobot>>();
    case PlayerType::ImproveGreedyRobot:
      return std::make_unique<RobotWapper<ImproveGreedyRobot>>();
    case PlayerType::SimulationRobot:
      return std::make_unique<RobotWapper<SimulationRobot>>();
    case PlayerType::MonteCarloRobot:
      return std::make_unique<RobotWapper<MonteCarloRobot>>();
    case PlayerType::ParallelSearchRobot:
      return std::make_unique<RobotWapper<ParallelSearchRobot>>();
    case PlayerType::Human:
      break;
    default:
      std::unreachable();
      break;
  }
  std::unreachable();
  return nullptr;
}

inline void MockRunningGame(PlayerType player1Type, PlayerType player2Type) {
  std::unique_ptr<Robot> robot1 = Robot::Create(player1Type);
  std::unique_ptr<Robot> robot2 = Robot::Create(player2Type);
  Assert(robot1);
  Assert(robot2);

  Random random;
  LoggingBoard board;
  while (board.Gaming()) {
    if (board.IsPlayer1Turn()) {
      board.Add(random.Choice(robot1->BestCandidateEdges(board)));
    } else {
      board.Add(random.Choice(robot2->BestCandidateEdges(board)));
    }
  }
}

}  // namespace __detail__::robot

using __detail__::robot::MockRunningGame;
using __detail__::robot::Robot;

}  // namespace dab
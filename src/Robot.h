#pragma once

#include "Board.h"
#include "PlayerType.h"
#include "Robot/GreedyRobot.h"
#include "Robot/ImproveGreedyRobot.h"
#include "Robot/MonteCarloRobot.h"
#include "Robot/ParallelSearchRobot.h"
#include "Robot/SimulationRobot.h"

namespace dab::__detail__ {

namespace robot {

class Robot {
 public:
  virtual ~Robot() = default;

  virtual Edge Move(const GameBoard& board) = 0;

  static std::unique_ptr<Robot> Create(PlayerType playerType);
};

template <typename Derived>
class RobotWrapper : public Robot, public Derived {
 public:
  using Derived::Derived;
  ~RobotWrapper() override = default;

  Edge Move(const GameBoard& board) override { return Random().Choice(Derived::BestCandidateEdges(board)); }
};

inline std::unique_ptr<Robot> Robot::Create(PlayerType playerType) {
  switch (playerType) {
    case PlayerType::GreedyRobot:
      return std::make_unique<RobotWrapper<GreedyRobot>>();
    case PlayerType::ImproveGreedyRobot:
      return std::make_unique<RobotWrapper<ImproveGreedyRobot>>();
    case PlayerType::SimulationRobot:
      return std::make_unique<RobotWrapper<SimulationRobot>>();
    case PlayerType::MonteCarloRobot:
      return std::make_unique<RobotWrapper<MonteCarloRobot>>();
    case PlayerType::ParallelSearchRobot:
      return std::make_unique<RobotWrapper<ParallelSearchRobot>>();
    case PlayerType::Human:
      return nullptr;
    default:
      std::unreachable();
  }
}

}  // namespace robot

using robot::Robot;

}  // namespace dab::__detail__
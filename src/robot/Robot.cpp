#include "Robot.h"

#include "GreedyRobot.h"
#include "ImproveGreedyRobot.h"
#include "MonteCarloRobot.h"
#include "ParallelSearchRobot.h"
#include "SimulationRobot.h"

namespace dab::__detail__::robot {

template <typename Derived>
class RobotWrapper : public Robot, public Derived {
 public:
  using Derived::Derived;
  ~RobotWrapper() override = default;

  model::Edge Move(const board::GameBoard& board) override {
    return iterable::Random().Choice(Derived::BestCandidateEdges(board));
  }
};

std::unique_ptr<Robot> Robot::Create(PlayerType playerType) {
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

}  // namespace dab::__detail__::robot
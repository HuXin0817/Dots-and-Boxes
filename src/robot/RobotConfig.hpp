#pragma once

#include <string>

#include "../common/Ptr.hpp"
#include "BasicSearchRobot.hpp"
#include "ImprovedSearchRobot.hpp"
#include "MonteCarloSearchRobot.hpp"
#include "ParallelSearchRobot.hpp"
#include "Robot.hpp"
#include "SimpleStrategyRobot.hpp"

enum class RobotType { L0, L1, L2, L3, L4 };

template <int BoardSize>
class RobotConfig {
  public:
  static Ptr<Robot<BoardSize>>
  CreateRobot(RobotType type) {
    switch (type) {
      case RobotType::L0:
        return new SimpleStrategyRobot<BoardSize>();
      case RobotType::L1:
        return new BasicSearchRobot<BoardSize>();
      case RobotType::L2:
        return new ImprovedSearchRobot<BoardSize>();
      case RobotType::L3:
        return new MonteCarloSearchRobot<BoardSize>();
      case RobotType::L4:
        return new ParallelSearchRobot<BoardSize>();
      default:
        return new ParallelSearchRobot<BoardSize>();
    }
  }

  static std::string
  GetRobotName(RobotType type) {
    switch (type) {
      case RobotType::L0:
        return "Simple Strategy Robot";
      case RobotType::L1:
        return "Basic Search Robot";
      case RobotType::L2:
        return "Improved Search Robot";
      case RobotType::L3:
        return "MonteCarlo Search Robot";
      case RobotType::L4:
        return "Parallel Search Robot";
      default:
        return "Null Robot";
    }
  }

  static RobotType
  ParseRobotType(const std::string& name) {
    if (name == "L0" || name == "l0") {
      return RobotType::L0;
    } else if (name == "L1" || name == "l1") {
      return RobotType::L1;
    } else if (name == "L2" || name == "l2") {
      return RobotType::L2;
    } else if (name == "L3" || name == "l3") {
      return RobotType::L3;
    } else if (name == "L4" || name == "l4") {
      return RobotType::L4;
    } else {
      return RobotType::L4;
    }
  }
};

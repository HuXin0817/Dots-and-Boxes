#pragma once

#include "../src/robot/GreedyRobot.hpp"
#include "../src/robot/MinimaxRobot.hpp"
#include "../src/robot/MonteCarloRobot.hpp"
#include "../src/robot/ParallelSearchRobot.hpp"
#include "../src/robot/PlayerType.hpp"
#include "../src/robot/Robot.hpp"
#include "../src/robot/SimulationRobot.hpp"

namespace dab {

using detail::robot::CreateRobot;
using detail::robot::GetPlayerTypeString;
using detail::robot::PlayerType;
using detail::robot::Robot;

}  // namespace dab
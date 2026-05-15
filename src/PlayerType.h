#pragma once

namespace dab {

enum class PlayerType {
  Human = 0,
  GreedyRobot,
  ImproveGreedyRobot,
  SimulationRobot,
  MonteCarloRobot,
  ParallelSearchRobot,
};

static constexpr const char* PlayerTypeOptionStrings[] = {
    "Human",
    "GreedyRobot",
    "ImproveGreedyRobot",
    "SimulationRobot",
    "MonteCarloRobot",
    "ParallelSearchRobot",
};

static constexpr PlayerType DefaultPlayerType = PlayerType::ParallelSearchRobot;

constexpr bool PlayerTypeIsRobot(PlayerType playerType) { return playerType != PlayerType::Human; }

}  // namespace dab
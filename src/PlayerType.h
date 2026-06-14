#pragma once

namespace dab {

enum class PlayerType {
  Human,
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

static constexpr const char* DefaultPlayerTypeString = PlayerTypeOptionStrings[static_cast<int>(DefaultPlayerType)];

static constexpr bool PlayerTypeIsRobot(PlayerType playerType) { return playerType != PlayerType::Human; }

}  // namespace dab
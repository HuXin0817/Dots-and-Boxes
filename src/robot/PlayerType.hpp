#pragma once

#include "ParallelSearchRobot.hpp"

namespace dab {

enum class PlayerType : uint8_t {
  Human = 0,
  GreedyRobot,
  MinimaxRobot,
  SimulationRobot,
  MonteCarloRobot,
  ParallelSearchRobot,
};

inline bool
PlayerTypeIsRobot(const PlayerType playerType) {
  return playerType != PlayerType::Human;
}

inline const char*
GetPlayerTypeString(const PlayerType playerType) {
  switch (playerType) {
    case PlayerType::Human:
      return "Human";
    case PlayerType::GreedyRobot:
      return "GreedyRobot";
    case PlayerType::MinimaxRobot:
      return "MinimaxRobot";
    case PlayerType::SimulationRobot:
      return "SimulationRobot";
    case PlayerType::MonteCarloRobot:
      return "MonteCarloRobot";
    case PlayerType::ParallelSearchRobot:
      return "ParallelSearchRobot";
  }
  return "";
}

template <int64_t BoardSize>
Robot<BoardSize>*
CreateRobot(const PlayerType playerType) {
  switch (playerType) {
    case PlayerType::GreedyRobot:
      return new GreedyRobot<BoardSize>();
    case PlayerType::MinimaxRobot:
      return new MinimaxRobot<BoardSize>();
    case PlayerType::SimulationRobot:
      return new SimulationRobot<BoardSize>();
    case PlayerType::MonteCarloRobot:
      return new MonteCarloRobot<BoardSize>();
    case PlayerType::ParallelSearchRobot:
      return new ParallelSearchRobot<BoardSize>();
    case PlayerType::Human:
      break;
  }
  return nullptr;
}

}  // namespace dab

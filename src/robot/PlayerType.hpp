#pragma once

#include "ParallelSearchRobot.hpp"

namespace dab::detail::robot {

enum class PlayerType {
  Human = 0,
  GreedyRobot,
  ImproveGreedyRobot,
  SimulationRobot,
  MonteCarloRobot,
  ParallelSearchRobot,
};

static constexpr const char* PlayerTypeString[] = {
    "Human",
    "GreedyRobot",
    "ImproveGreedyRobot",
    "SimulationRobot",
    "MonteCarloRobot",
    "ParallelSearchRobot",
};

inline bool PlayerTypeIsRobot(const PlayerType playerType) { return playerType != PlayerType::Human; }

template <int64_t BoardSize>
Robot<BoardSize>* CreateRobot(const PlayerType playerType) {
  switch (playerType) {
    case PlayerType::GreedyRobot:
      return new GreedyRobot<BoardSize>();
    case PlayerType::ImproveGreedyRobot:
      return new ImproveGreedyRobot<BoardSize>();
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

}  // namespace dab::detail::robot
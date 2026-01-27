#pragma once

#include "ParallelSearchRobot.hpp"

namespace dab {

enum class PlayerType : uint8_t {
  Human = 0,
  GreedyRobot,
  MinimaxRobot,
  AlphaBetaRobot,
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
      return "dab::PlayerType::Human";
    case PlayerType::GreedyRobot:
      return "dab::PlayerType::GreedyRobot";
    case PlayerType::MinimaxRobot:
      return "dab::PlayerType::MinimaxRobot";
    case PlayerType::AlphaBetaRobot:
      return "dab::PlayerType::AlphaBetaRobot";
    case PlayerType::MonteCarloRobot:
      return "dab::PlayerType::MonteCarloRobot";
    case PlayerType::ParallelSearchRobot:
      return "dab::PlayerType::ParallelSearchRobot";
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
    case PlayerType::AlphaBetaRobot:
      return new AlphaBetaRobot<BoardSize>();
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

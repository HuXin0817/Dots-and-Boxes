#pragma once

#include "ParallelSearchRobot.hpp"

namespace dab {

enum class PlayerType : uint8_t {
  Human = 0,
  SimpleStrategyRobot,
  BasicSearchRobot,
  ImprovedSearchRobot,
  MonteCarloSearchRobot,
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
    case PlayerType::SimpleStrategyRobot:
      return "SimpleStrategyRobot";
    case PlayerType::BasicSearchRobot:
      return "BasicSearchRobot";
    case PlayerType::ImprovedSearchRobot:
      return "ImprovedSearchRobot";
    case PlayerType::MonteCarloSearchRobot:
      return "MonteCarloSearchRobot";
    case PlayerType::ParallelSearchRobot:
      return "ParallelSearchRobot";
  }
  return "";
}

template <int64_t BoardSize>
Robot<BoardSize>*
CreateRobot(const PlayerType playerType) {
  switch (playerType) {
    case PlayerType::SimpleStrategyRobot:
      return new SimpleStrategyRobot<BoardSize>();
    case PlayerType::BasicSearchRobot:
      return new BasicSearchRobot<BoardSize>();
    case PlayerType::ImprovedSearchRobot:
      return new ImprovedSearchRobot<BoardSize>();
    case PlayerType::MonteCarloSearchRobot:
      return new MonteCarloSearchRobot<BoardSize>();
    case PlayerType::ParallelSearchRobot:
      return new ParallelSearchRobot<BoardSize>();
    case PlayerType::Human:
      break;
  }
  return nullptr;
}

}  // namespace dab

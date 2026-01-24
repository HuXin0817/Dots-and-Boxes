#pragma once

#include "ParallelSearchRobot.hpp"

enum class PlayerType {
  Human = 0,
  SimpleStrategyRobot,
  BasicSearchRobot,
  ImprovedSearchRobot,
  MonteCarloSearchRobot,
  ParallelSearchRobot,
};

inline bool
PlayerTypeIsRobot(const PlayerType PlayerType) {
  return PlayerType != PlayerType::Human;
}

inline const char*
GetPlayerTypeString(const PlayerType type) {
  switch (type) {
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
CreateRobot(const PlayerType PlayerType) {
  switch (PlayerType) {
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

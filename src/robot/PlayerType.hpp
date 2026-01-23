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

static bool
PlayerTypeIsRobot(const PlayerType PlayerType) {
  return PlayerType != PlayerType::Human;
}

template <int64_t BoardSize>
static std::unique_ptr<Robot<BoardSize>>
CreateRobot(const PlayerType PlayerType) {
  switch (PlayerType) {
    case PlayerType::SimpleStrategyRobot:
      return std::make_unique<SimpleStrategyRobot<BoardSize>>();
    case PlayerType::BasicSearchRobot:
      return std::make_unique<BasicSearchRobot<BoardSize>>();
    case PlayerType::ImprovedSearchRobot:
      return std::make_unique<ImprovedSearchRobot<BoardSize>>();
    case PlayerType::MonteCarloSearchRobot:
      return std::make_unique<MonteCarloSearchRobot<BoardSize>>();
    case PlayerType::ParallelSearchRobot:
      return std::make_unique<ParallelSearchRobot<BoardSize>>();
    case PlayerType::Human:
      break;
  }
  return nullptr;
}

static const char*
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

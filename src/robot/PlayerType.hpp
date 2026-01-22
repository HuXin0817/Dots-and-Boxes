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

template <int BoardSize, typename SizeType>
static std::unique_ptr<Robot<BoardSize, SizeType>>
CreateRobot(const PlayerType PlayerType) {
  switch (PlayerType) {
    case PlayerType::SimpleStrategyRobot:
      return std::make_unique<SimpleStrategyRobot<BoardSize, SizeType>>();
    case PlayerType::BasicSearchRobot:
      return std::make_unique<BasicSearchRobot<BoardSize, SizeType>>();
    case PlayerType::ImprovedSearchRobot:
      return std::make_unique<ImprovedSearchRobot<BoardSize, SizeType>>();
    case PlayerType::MonteCarloSearchRobot:
      return std::make_unique<MonteCarloSearchRobot<BoardSize, SizeType>>();
    case PlayerType::ParallelSearchRobot:
      return std::make_unique<ParallelSearchRobot<BoardSize, SizeType>>();
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

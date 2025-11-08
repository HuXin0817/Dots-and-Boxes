#pragma once

#include <string>

enum class PlayerType {
  Human,
  Robot,
};

class PlayerConfig {
  public:
  static std::string
  GetPlayerTypeString(PlayerType type) {
    switch (type) {
      case PlayerType::Human:
        return "Human";
      case PlayerType::Robot:
        return "Robot";
    }
    return "";
  }

  static PlayerType
  ParsePlayerType(const std::string& playerType) {
    if (playerType == "human") {
      return PlayerType::Human;
    }
    return PlayerType::Robot;
  }
};

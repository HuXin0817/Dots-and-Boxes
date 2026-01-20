#pragma once

namespace dab::frontend {

enum class PlayerType {
  Human,
  Robot,
};

static const char*
GetPlayerTypeString(const PlayerType type) {
  switch (type) {
    case PlayerType::Human:
      return "Human";
    case PlayerType::Robot:
      return "Robot";
  }
  return "";
}

}  // namespace dab::frontend

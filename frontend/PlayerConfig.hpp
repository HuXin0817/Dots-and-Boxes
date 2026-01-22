#pragma once

#include <QString>
#include <optional>

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

static std::optional<dab::frontend::PlayerType>
parsePlayerType(const QString& str) {
  if (str.compare("human", Qt::CaseInsensitive) == 0) {
    return dab::frontend::PlayerType::Human;
  } else if (str.compare("robot", Qt::CaseInsensitive) == 0) {
    return dab::frontend::PlayerType::Robot;
  }
  return std::nullopt;
}

}  // namespace dab::frontend

#pragma once

#include <QString>

#include "../src/robot/PlayerType.hpp"

namespace dab::frontend {

class Config {
  public:
  Config(int64_t boardSize, PlayerType player1Type, PlayerType player2Type);

  QString
  ToString() const;

  int64_t BoardSize;
  PlayerType Player1Type;
  PlayerType Player2Type;
};

inline Config::Config(int64_t boardSize, PlayerType player1Type, PlayerType player2Type)
    : BoardSize(boardSize), Player1Type(player1Type), Player2Type(player2Type) {
}

inline QString
Config::ToString() const {
  return QString(R"({"BoardSize":%1,"Player1":"%2","Player2":"%3"})")
      .arg(BoardSize)
      .arg(GetPlayerTypeString(Player1Type))
      .arg(GetPlayerTypeString(Player2Type));
}

}  // namespace dab::frontend

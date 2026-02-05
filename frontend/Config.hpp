#pragma once

#include <Dab/Robot.hpp>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <cstdint>

namespace dab::detail::frontend {

class Config {
  public:
  Config(int64_t boardSize, PlayerType player1Type, PlayerType player2Type, bool backgroundMode);

  QString
  ToString() const;

  int64_t BoardSize;
  PlayerType Player1Type;
  PlayerType Player2Type;
  bool BackgroundMode;
};

inline Config::Config(int64_t boardSize, PlayerType player1Type, PlayerType player2Type, bool backgroundMode)
    : BoardSize(boardSize), Player1Type(player1Type), Player2Type(player2Type), BackgroundMode(backgroundMode) {
}

inline QString
Config::ToString() const {
  QJsonObject configData;
  configData.insert("BoardSize", BoardSize);
  configData.insert("Player1Type", PlayerTypeString[static_cast<int>(Player1Type)]);
  configData.insert("Player2Type", PlayerTypeString[static_cast<int>(Player2Type)]);
  configData.insert("BackgroundMode", BackgroundMode);
  QJsonObject config;
  config.insert("Config", configData);
  return QJsonDocument(config).toJson(QJsonDocument::Compact);
}

}  // namespace dab::detail::frontend
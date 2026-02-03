#pragma once

#include "MainWindow.hpp"

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

template <int64_t MaxBoardSize>
class MainWindowCreator {
  public:
  QPointer<QWidget>
  CreateMainWindow(const Config& config, QPointer<QWidget> parent = nullptr);

  private:
  template <int64_t BoardSize>
  QPointer<QWidget>
  CreateMainWindowImpl(const Config& config, QPointer<QWidget> parent);
};

template <int64_t MaxBoardSize>
inline QPointer<QWidget>
MainWindowCreator<MaxBoardSize>::CreateMainWindow(const Config& config, QPointer<QWidget> parent) {
  assert(config.BoardSize > 0 && config.BoardSize <= MaxBoardSize);
  return CreateMainWindowImpl<MaxBoardSize>(config, parent);
}

template <int64_t MaxBoardSize>
template <int64_t BoardSize>
QPointer<QWidget>
MainWindowCreator<MaxBoardSize>::CreateMainWindowImpl(const Config& config, QPointer<QWidget> parent) {
  if (config.BoardSize == BoardSize) {
    return new MainWindow<BoardSize>(config.Player1Type, config.Player2Type, config.BoardSize, parent);
  }
  if constexpr (BoardSize > 1) {
    return CreateMainWindowImpl<BoardSize - 1>(config, parent);
  }
  return nullptr;
}

}  // namespace dab::detail::frontend
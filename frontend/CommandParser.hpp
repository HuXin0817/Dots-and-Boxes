#pragma once

#include <QCommandLineParser>

#include "MainWindow.hpp"

namespace dab::detail::frontend {

static constexpr int64_t DefaultBoardSize = 6;
static constexpr int64_t MinBoardSize = 2;
static constexpr int64_t MaxBoardSize = 36;

static constexpr PlayerType DefaultPlayerType = PlayerType::ParallelSearchRobot;
static constexpr const char* PlayerTypeOptionStrings[] = {
    "human",
    "robot:easy",
    "robot:medium",
    "robot:hard",
    "robot:expert",
    "robot:master",
};

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
  QJsonObject configData;
  configData.insert("BoardSize", BoardSize);
  configData.insert("Player1Type", GetPlayerTypeString(Player1Type));
  configData.insert("Player2Type", GetPlayerTypeString(Player2Type));
  QJsonObject config;
  config.insert("Config", configData);
  return QJsonDocument(config).toJson(QJsonDocument::Compact);
}

class CommandParser {
  public:
  Config
  Process(const QApplication& application);

  private:
  static constexpr size_t PlayerTypeOptionStringsSize =
      sizeof(PlayerTypeOptionStrings) / sizeof(PlayerTypeOptionStrings[0]);

  QCommandLineOption
  BoardSizeOption();
  QCommandLineOption
  PlayerTypeOption(int8_t player);
  int64_t
  ParseBoardSize(const QString& arg);
  PlayerType
  ParsePlayerType(const QString& arg);
};

inline Config
CommandParser::Process(const QApplication& application) {
  const QCommandLineOption boardSizeOption = BoardSizeOption();
  const QCommandLineOption player1Option = PlayerTypeOption(1);
  const QCommandLineOption player2Option = PlayerTypeOption(2);

  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addOption(boardSizeOption);
  parser.addOption(player1Option);
  parser.addOption(player2Option);
  parser.process(application);

  Config Config(ParseBoardSize(parser.value(boardSizeOption)),
                ParsePlayerType(parser.value(player1Option)),
                ParsePlayerType(parser.value(player2Option)));
  qInfo() << Config.ToString().toLocal8Bit().constData();
  return Config;
}

inline QCommandLineOption
CommandParser::BoardSizeOption() {
  return QCommandLineOption(QStringList() << "s" << "size",
                            QString("Set board size ranging from [%1, %2] (default: %3).")
                                .arg(MinBoardSize)
                                .arg(MaxBoardSize)
                                .arg(DefaultBoardSize),
                            "BoardSize",
                            QString::number(DefaultBoardSize));
}

inline QCommandLineOption
CommandParser::PlayerTypeOption(int8_t player) {
  QStringList accepted;
  accepted << "human" << "robot";
  for (size_t i = 1; i < PlayerTypeOptionStringsSize; ++i) {
    accepted << QString::fromUtf8(PlayerTypeOptionStrings[i]);
  }

  QString acceptedStr;
  if (accepted.size() == 1) {
    acceptedStr = "'" + accepted[0] + "'";
  } else if (accepted.size() == 2) {
    acceptedStr = "'" + accepted[0] + "' or '" + accepted[1] + "'";
  } else {
    QString allButLast = accepted.mid(0, accepted.size() - 1).join("', '");
    acceptedStr = "'" + allButLast + "' or '" + accepted.last() + "'";
  }

  return QCommandLineOption(
      QStringList() << QString("p%1").arg(player) << QString("player%1").arg(player),
      QString("Set type of player %1. Accepts: %2 (default: 'robot'). Note: 'robot' is equivalent to '%3'.")
          .arg(player)
          .arg(acceptedStr)
          .arg(QString::fromUtf8(PlayerTypeOptionStrings[static_cast<int>(DefaultPlayerType)])),
      "Type",
      "robot");
}

inline int64_t
CommandParser::ParseBoardSize(const QString& arg) {
  bool conversionOk = false;
  const int64_t boardSize = arg.toLongLong(&conversionOk);
  if (!conversionOk || boardSize < MinBoardSize || boardSize > MaxBoardSize) {
    qInfo("Error: Invalid board size. Must be an integer in range [%lld, %lld].", MinBoardSize, MaxBoardSize);
    exit(EXIT_FAILURE);
  }
  return boardSize;
}

inline PlayerType
CommandParser::ParsePlayerType(const QString& arg) {
  if (arg.compare("robot", Qt::CaseInsensitive) == 0) {
    return DefaultPlayerType;
  }
  for (size_t i = 0; i < PlayerTypeOptionStringsSize; ++i) {
    if (arg.compare(PlayerTypeOptionStrings[i], Qt::CaseInsensitive) == 0) {
      return static_cast<PlayerType>(i);
    }
  }
  qInfo("Error: Invalid player type '%s'.", arg.toLocal8Bit().constData());
  exit(EXIT_FAILURE);
}

class MainWindowCreator {
  public:
  QPointer<QWidget>
  CreateMainWindow(const Config& config, QPointer<QWidget> parent = nullptr);

  private:
  template <int64_t BoardSize>
  QPointer<QWidget>
  CreateMainWindowImpl(const Config& config, QPointer<QWidget> parent);
};

template <int64_t BoardSize>
QPointer<QWidget>
MainWindowCreator::CreateMainWindowImpl(const Config& config, QPointer<QWidget> parent) {
  if (config.BoardSize == BoardSize) {
    return new MainWindow<BoardSize>(config.Player1Type, config.Player2Type, parent);
  }
  if constexpr (BoardSize > MinBoardSize) {
    return CreateMainWindowImpl<BoardSize - 1>(config, parent);
  }
  return nullptr;
}

inline QPointer<QWidget>
MainWindowCreator::CreateMainWindow(const Config& config, QPointer<QWidget> parent) {
  if (config.BoardSize < MinBoardSize || config.BoardSize > MaxBoardSize) {
    return nullptr;
  }
  return CreateMainWindowImpl<MaxBoardSize>(config, parent);
}

}  // namespace dab::detail::frontend
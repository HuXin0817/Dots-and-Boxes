#pragma once

#include <QCommandLineParser>
#include <cstdint>
#include <cstdlib>

#include "MainWindow.hpp"

namespace dab::detail::frontend {

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
    return new MainWindow<BoardSize>(config.Player1Type, config.Player2Type, config.BackgroundMode, parent);
  }
  if constexpr (BoardSize > 1) {
    return CreateMainWindowImpl<BoardSize - 1>(config, parent);
  }
  return nullptr;
}

template <int64_t MaxBoardSize = 36,
          int64_t DefaultBoardSize = 6,
          PlayerType DefaultPlayerType = PlayerType::ParallelSearchRobot>
class CommandParser {
  static_assert(DefaultBoardSize <= MaxBoardSize);

  public:
  CommandParser() = default;

  int
  Process(const QApplication& application);

  private:
  static constexpr size_t PlayerTypeOptionStringsSize =
      sizeof(PlayerTypeOptionStrings) / sizeof(PlayerTypeOptionStrings[0]);

  QCommandLineOption
  BoardSizeOption();
  QCommandLineOption
  PlayerTypeOption(int8_t player);
  QCommandLineOption
  BackgroundModeOption();
  int64_t
  ParseBoardSize(const QString& arg);
  PlayerType
  ParsePlayerType(const QString& arg);
};

template <int64_t MaxBoardSize, int64_t DefaultBoardSize, PlayerType DefaultPlayerType>
inline int
CommandParser<MaxBoardSize, DefaultBoardSize, DefaultPlayerType>::Process(const QApplication& application) {
  const QCommandLineOption boardSizeOption = BoardSizeOption();
  const QCommandLineOption player1Option = PlayerTypeOption(1);
  const QCommandLineOption player2Option = PlayerTypeOption(2);
  const QCommandLineOption backgroundModeOption = BackgroundModeOption();

  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addOption(boardSizeOption);
  parser.addOption(player1Option);
  parser.addOption(player2Option);
  parser.addOption(backgroundModeOption);
  parser.process(application);

  Config Config(ParseBoardSize(parser.value(boardSizeOption)),
                ParsePlayerType(parser.value(player1Option)),
                ParsePlayerType(parser.value(player2Option)),
                parser.isSet(backgroundModeOption));
  if (Config.BackgroundMode && (!PlayerTypeIsRobot(Config.Player1Type) || !PlayerTypeIsRobot(Config.Player2Type))) {
    qInfo("Error: player type 'human' is not allow in background mode.");
    exit(EXIT_FAILURE);
  }

  qInfo() << Config.ToString().toLocal8Bit().constData();

  QPointer<QWidget> MainWindow = MainWindowCreator<MaxBoardSize>().CreateMainWindow(Config);
  MainWindow->show();
  return application.exec();
}

template <int64_t MaxBoardSize, int64_t DefaultBoardSize, PlayerType DefaultPlayerType>
inline QCommandLineOption
CommandParser<MaxBoardSize, DefaultBoardSize, DefaultPlayerType>::BoardSizeOption() {
  return QCommandLineOption(
      QStringList() << "s" << "size",
      QString("Set board size ranging from [1, %2] (default: %3).").arg(MaxBoardSize).arg(DefaultBoardSize),
      "BoardSize",
      QString::number(DefaultBoardSize));
}

template <int64_t MaxBoardSize, int64_t DefaultBoardSize, PlayerType DefaultPlayerType>
inline QCommandLineOption
CommandParser<MaxBoardSize, DefaultBoardSize, DefaultPlayerType>::PlayerTypeOption(int8_t player) {
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

template <int64_t MaxBoardSize, int64_t DefaultBoardSize, PlayerType DefaultPlayerType>
inline QCommandLineOption
CommandParser<MaxBoardSize, DefaultBoardSize, DefaultPlayerType>::BackgroundModeOption() {
  return QCommandLineOption(QStringList() << "b" << "background", "Run in background mode.");
}

template <int64_t MaxBoardSize, int64_t DefaultBoardSize, PlayerType DefaultPlayerType>
inline int64_t
CommandParser<MaxBoardSize, DefaultBoardSize, DefaultPlayerType>::ParseBoardSize(const QString& arg) {
  bool conversionOk = false;
  const int64_t boardSize = arg.toLongLong(&conversionOk);
  if (!conversionOk || boardSize <= 0 || boardSize > MaxBoardSize) {
    qInfo("Error: Invalid board size. Must be an integer in range [1, %lld].", MaxBoardSize);
    exit(EXIT_FAILURE);
  }
  return boardSize;
}

template <int64_t MaxBoardSize, int64_t DefaultBoardSize, PlayerType DefaultPlayerType>
inline PlayerType
CommandParser<MaxBoardSize, DefaultBoardSize, DefaultPlayerType>::ParsePlayerType(const QString& arg) {
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

}  // namespace dab::detail::frontend
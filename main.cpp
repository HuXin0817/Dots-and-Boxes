#include <QApplication>
#include <QCommandLineParser>
#include <cstdlib>
#include <print>
#include <ranges>

#include "frontend/Frontend.h"

using namespace dab;

static constexpr std::int64_t DefaultBoardSize = __DefaultBoardSize__;
static constexpr std::int64_t MaxBoardSize = __MaxBoardSize__;
static_assert(DefaultBoardSize <= MaxBoardSize);

QCommandLineOption CreateBoardSizeOption() {
  const QByteArray boardSizeEnv = qgetenv("BOARD_SIZE");
  QString defaultBoardSize = QString::number(DefaultBoardSize);
  if (!boardSizeEnv.isEmpty()) {
    defaultBoardSize = boardSizeEnv;
  }
  const QStringList names = {"s", "boardsize"};
  const QString description = QString("Set board size (1-%1).").arg(MaxBoardSize);
  return {names, description, "size", defaultBoardSize};
}

QCommandLineOption CreatePlayerTypeOption(int playerId) {
  const QByteArray playerTypeEnv = qgetenv(QString("PLAYER%1").arg(playerId).toLocal8Bit().constData());
  QString defaultPlayerType = "robot";
  if (!playerTypeEnv.isEmpty()) {
    defaultPlayerType = playerTypeEnv;
  }

  QStringList accepted = {"human", "robot"};
  for (const QString option : PlayerTypeOptionStrings) {
    accepted << option;
  }

  QString acceptedStr;
  if (accepted.size() == 1) {
    acceptedStr = "'" + accepted[0] + "'";
  } else if (accepted.size() == 2) {
    acceptedStr = "'" + accepted[0] + "' or '" + accepted[1] + "'";
  } else {
    const QString allButLast = accepted.mid(0, accepted.size() - 1).join("', '");
    acceptedStr = "'" + allButLast + "' or '" + accepted.last() + "'";
  }

  const QStringList names = {QString("p%1").arg(playerId), QString("player%1").arg(playerId)};
  const QString discription =
      QString("Set type of player %1. Accepts: %2 (default: 'robot'). Note: 'robot' is equivalent to '%3'.")
          .arg(playerId)
          .arg(acceptedStr)
          .arg(DefaultPlayerTypeString);
  return {names, discription, "type", defaultPlayerType};
}

PlayerType ParsePlayerType(const QString& arg) {
  if (arg.compare("robot", Qt::CaseSensitivity::CaseInsensitive) == 0) {
    return DefaultPlayerType;
  }
  for (const std::size_t i : std::views::iota(static_cast<std::size_t>(0), std::size(PlayerTypeOptionStrings))) {
    if (arg.compare(PlayerTypeOptionStrings[i], Qt::CaseSensitivity::CaseInsensitive) == 0) {
      return static_cast<PlayerType>(i);
    }
  }
  std::println(stderr, "Invalid player type '{}'.", arg.toLocal8Bit().constData());
  exit(EXIT_FAILURE);
}

int64_t ParseBoardSize(const QString& arg) {
  bool ok = false;
  const std::int64_t boardSize = arg.toLongLong(&ok);
  if (!ok || boardSize < 1 || boardSize > MaxBoardSize) {
    std::println(
        stderr, "Invalid board size '{}'. Must be between {} and {}.", arg.toLocal8Bit().constData(), 1, MaxBoardSize);
    exit(EXIT_FAILURE);
  }
  return boardSize;
}

int main(int argc, char* argv[]) {
  QApplication application(argc, argv);
  application.setApplicationName("Dots and Boxes");
  application.setApplicationVersion(__Version__);
  application.setOrganizationName("Dots and Boxes");

  const QCommandLineOption boardSizeOption = CreateBoardSizeOption();
  const QCommandLineOption player1Option = CreatePlayerTypeOption(1);
  const QCommandLineOption player2Option = CreatePlayerTypeOption(2);

  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addOption(boardSizeOption);
  parser.addOption(player1Option);
  parser.addOption(player2Option);
  parser.process(application);

  const std::int64_t boardSize = ParseBoardSize(parser.value(boardSizeOption));
  const PlayerType player1Type = ParsePlayerType(parser.value(player1Option));
  const PlayerType player2Type = ParsePlayerType(parser.value(player2Option));

  QWidget* mainWindow = CreateMainWindow<MaxBoardSize>(boardSize, player1Type, player2Type);
  mainWindow->show();
  const int code = application.exec();
  if (code != 0) {
    std::println(stderr, "Exit code {}.", code);
  }
  return code;
}
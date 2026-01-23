#include "frontend/MainWindowCreator.hpp"

static constexpr int64_t DefaultBoardSize = 6;

QCommandLineOption
BoardSizeOption() {
  return QCommandLineOption(QStringList() << "s" << "size",
                            QString("Set board size ranging from [%1, %2] (default: %3).")
                                .arg(MainWindowCreator::MinBoardSize)
                                .arg(MainWindowCreator::MaxBoardSize)
                                .arg(DefaultBoardSize),
                            "BoardSize",
                            QString::number(DefaultBoardSize));
}

QCommandLineOption
PlayerTypeOption(int player) {
  const char* discription =
      "Set type of player %1. Accepts: 'human', 'robot', 'robot:easy', 'robot:medium', 'robot:hard', 'robot:expert',"
      " or 'robot:master' (default: robot). Note: 'robot' is equivalent to 'robot:master'.";

  return QCommandLineOption(QStringList() << QString("p%1").arg(player) << QString("player%1").arg(player),
                            QString(discription).arg(player),
                            "Type",
                            "robot");
}

int64_t
ParseBoardSize(const QString& str) {
  bool conversionOk = false;
  int64_t boardSize = str.toLongLong(&conversionOk);

  if (!conversionOk || boardSize < MainWindowCreator::MinBoardSize || boardSize > MainWindowCreator::MaxBoardSize) {
    std::fprintf(stderr,
                 "Error: Invalid board size. Must be an integer in range [%lld, %lld].\n",
                 MainWindowCreator::MinBoardSize,
                 MainWindowCreator::MaxBoardSize);
    exit(EXIT_FAILURE);
  }

  return boardSize;
}

PlayerType
ParsePlayerType(const QString& str) {
  if (str.compare("human", Qt::CaseInsensitive) == 0) {
    return PlayerType::Human;
  } else if (str.compare("robot", Qt::CaseInsensitive) == 0) {
    return PlayerType::ParallelSearchRobot;
  } else if (str.compare("robot:easy", Qt::CaseInsensitive) == 0) {
    return PlayerType::SimpleStrategyRobot;
  } else if (str.compare("robot:medium", Qt::CaseInsensitive) == 0) {
    return PlayerType::BasicSearchRobot;
  } else if (str.compare("robot:hard", Qt::CaseInsensitive) == 0) {
    return PlayerType::ImprovedSearchRobot;
  } else if (str.compare("robot:expert", Qt::CaseInsensitive) == 0) {
    return PlayerType::MonteCarloSearchRobot;
  } else if (str.compare("robot:master", Qt::CaseInsensitive) == 0) {
    return PlayerType::ParallelSearchRobot;
  }
  std::fprintf(stderr, "Error: Invalid player type '%s'.\n", str.toLocal8Bit().constData());
  exit(EXIT_FAILURE);
}

int
main(int argc, char* argv[]) {
  QApplication application(argc, argv);

  application.setApplicationName("Dots and Boxes");
  application.setApplicationVersion("1.0.0");
  application.setOrganizationName("Dots and Boxes");

  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addVersionOption();

  QCommandLineOption boardSizeOption = BoardSizeOption();
  parser.addOption(boardSizeOption);

  QCommandLineOption player1Option = PlayerTypeOption(1);
  parser.addOption(player1Option);

  QCommandLineOption player2Option = PlayerTypeOption(2);
  parser.addOption(player2Option);

  parser.process(application);

  int64_t boardSize = ParseBoardSize(parser.value(boardSizeOption));
  PlayerType player1Type = ParsePlayerType(parser.value(player1Option));
  PlayerType player2Type = ParsePlayerType(parser.value(player2Option));

  qInfo("Config: {\"BoardSize\":%lld,\"Player1\":\"%s\",\"Player2\":\"%s\"}",
        boardSize,
        GetPlayerTypeString(player1Type),
        GetPlayerTypeString(player2Type));

  if (QWidget* mainWindow = MainWindowCreator().CreateMainWindow(boardSize, player1Type, player2Type)) {
    mainWindow->show();
  }

  return application.exec();
}

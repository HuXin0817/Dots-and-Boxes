#include "frontend/MainWindowCreator.hpp"

static constexpr int DefaultBoardSize = 6;

int
main(int argc, char* argv[]) {
  QApplication application(argc, argv);

  application.setApplicationName("Dots and Boxes");
  application.setApplicationVersion("1.0.0");
  application.setOrganizationName("Dots and Boxes");

  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addVersionOption();

  QCommandLineOption boardSizeOption(QStringList() << "s" << "size",
                                     QString("Set board size, ranging from [%1, %2] with a default of %3.")
                                         .arg(dab::frontend::MainWindowCreator::MinBoardSize)
                                         .arg(dab::frontend::MainWindowCreator::MaxBoardSize)
                                         .arg(DefaultBoardSize),
                                     "BoardSize",
                                     QString::number(DefaultBoardSize));
  parser.addOption(boardSizeOption);

  QCommandLineOption player1Option(
      QStringList() << "p1" << "player1", "Set player 1 type: 'human' or 'robot' (default: 'robot').", "Type", "robot");
  parser.addOption(player1Option);

  QCommandLineOption player2Option(
      QStringList() << "p2" << "player2", "Set player 2 type: 'human' or 'robot' (default: 'robot').", "Type", "robot");
  parser.addOption(player2Option);

  parser.process(application);

  bool conversionOk = false;
  int boardSize = parser.value(boardSizeOption).toInt(&conversionOk);

  if (!conversionOk || boardSize < dab::frontend::MainWindowCreator::MinBoardSize ||
      boardSize > dab::frontend::MainWindowCreator::MaxBoardSize) {
    std::fprintf(stderr,
                 "Error: Invalid board size. Must be an integer in range [%d, %d].",
                 dab::frontend::MainWindowCreator::MinBoardSize,
                 dab::frontend::MainWindowCreator::MaxBoardSize);
    return EXIT_FAILURE;
  }

  std::optional<dab::frontend::PlayerType> player1TypeOpt = dab::frontend::parsePlayerType(parser.value(player1Option));
  if (!player1TypeOpt.has_value()) {
    std::fprintf(stderr, "Error: Invalid player1 type. Use 'human' or 'robot'.\n");
    return EXIT_FAILURE;
  }
  dab::frontend::PlayerType player1Type = player1TypeOpt.value();

  std::optional<dab::frontend::PlayerType> player2TypeOpt = dab::frontend::parsePlayerType(parser.value(player2Option));
  if (!player2TypeOpt.has_value()) {
    std::fprintf(stderr, "Error: Invalid player2 type. Use 'human' or 'robot'.\n");
    return EXIT_FAILURE;
  }
  dab::frontend::PlayerType player2Type = player2TypeOpt.value();

  qInfo("Config: {\"BoardSize\":%d,\"Player1\":\"%s\",\"Player2\":\"%s\"}",
        boardSize,
        dab::frontend::GetPlayerTypeString(player1Type),
        dab::frontend::GetPlayerTypeString(player2Type));

  if (QWidget* mainWindow = dab::frontend::MainWindowCreator().CreateMainWindow(boardSize, player1Type, player2Type)) {
    mainWindow->show();
  }

  return application.exec();
}

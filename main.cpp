#include <QCommandLineOption>
#include <QCommandLineParser>

#include "frontend/MainWindowCreator.hpp"

static constexpr int DefaultBoardSize = 6;
static constexpr dab::frontend::PlayerType Player1Type = dab::frontend::PlayerType::Robot;
static constexpr dab::frontend::PlayerType Player2Type = dab::frontend::PlayerType::Robot;

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
  parser.process(application);

  bool conversionOk = false;
  int boardSize = parser.value(boardSizeOption).toInt(&conversionOk);

  if (!conversionOk || boardSize < dab::frontend::MainWindowCreator::MinBoardSize ||
      boardSize > dab::frontend::MainWindowCreator::MaxBoardSize) {
    fprintf(stderr,
            "Error: Invalid board size. The board size must be an integer in range [%d, %d].\n",
            dab::frontend::MainWindowCreator::MinBoardSize,
            dab::frontend::MainWindowCreator::MaxBoardSize);
    return EXIT_FAILURE;
  }

  qDebug("Config: {\"BoardSize\":%d,\"Player1\":\"%s\",\"Player2\":\"%s\"}",
         boardSize,
         dab::frontend::GetPlayerTypeString(Player1Type),
         dab::frontend::GetPlayerTypeString(Player2Type));

  if (QWidget* mainWindow = dab::frontend::MainWindowCreator().CreateMainWindow(boardSize, Player1Type, Player2Type)) {
    mainWindow->show();
  }

  return application.exec();
}

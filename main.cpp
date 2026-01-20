#include "frontend/MainWindowCreator.hpp"

static constexpr dab::frontend::PlayerType Player1Type = dab::frontend::PlayerType::Robot;
static constexpr dab::frontend::PlayerType Player2Type = dab::frontend::PlayerType::Robot;

int
main(int argc, char* argv[]) {
  int BoardSize = 6;
  if (argc >= 2) {
    BoardSize = std::clamp(std::stoi(argv[1]), 2, dab::frontend::MainWindowCreator::MaxBoardSize);
  }

  QApplication application(argc, argv);

  application.setApplicationName("Dots and Boxes");
  application.setApplicationVersion("1.0");
  application.setOrganizationName("Dots and Boxes");

  qDebug("Starting game with player configuration:\n");
  qDebug("  Player 1: %s", dab::frontend::GetPlayerTypeString(Player1Type));
  qDebug("  Player 2: %s\n", dab::frontend::GetPlayerTypeString(Player2Type));

  if (QWidget* mainWindow = dab::frontend::MainWindowCreator().CreateMainWindow(BoardSize, Player1Type, Player2Type)) {
    mainWindow->show();
  }

  return application.exec();
}

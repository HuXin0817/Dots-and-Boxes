#include "frontend/MainWindowCreator.hpp"

static constexpr int BoardSize = 6;
static constexpr PlayerType Player1Type = PlayerType::Robot;
static constexpr PlayerType Player2Type = PlayerType::Robot;

int
main(int argc, char* argv[]) {
  QApplication application(argc, argv);

  application.setApplicationName("Dots and Boxes");
  application.setApplicationVersion("1.0");
  application.setOrganizationName("Dots and Boxes");

  qDebug("Starting game with player configuration:\n");
  qDebug("  Player 1: %s", PlayerConfig::GetPlayerTypeString(Player1Type).c_str());
  qDebug("  Player 2: %s\n", PlayerConfig::GetPlayerTypeString(Player2Type).c_str());

  if (auto mainWindow = MainWindowCreator().CreateMainWindow(BoardSize, Player1Type, Player2Type)) {
    mainWindow->show();
  }

  return application.exec();
}

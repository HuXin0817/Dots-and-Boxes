#include <QStyleFactory>

#include "frontend/MainWindow.hpp"
#include "src/robot/RobotConfig.hpp"

static constexpr RobotType Robot1Type = RobotType::L4;
static constexpr RobotType Robot2Type = RobotType::L4;
static constexpr PlayerType Player1Type = PlayerType::Robot;
static constexpr PlayerType Player2Type = PlayerType::Robot;

int
main(int argc, char* argv[]) {
  QApplication Application(argc, argv);

  Application.setApplicationName("Dots and Boxes");
  Application.setApplicationVersion("1.0");
  Application.setOrganizationName("Dots and Boxes");
  Application.setStyle(QStyleFactory::create("Fusion"));

  printf("Starting game with player configuration:");
  printf("\n  Player 1: %s", PlayerConfig::GetPlayerTypeString(Player1Type).c_str());
  if (Player1Type == PlayerType::Robot) {
    printf(" - %s", RobotConfig::GetRobotName(Robot1Type).c_str());
  }
  printf("\n  Player 2: %s", PlayerConfig::GetPlayerTypeString(Player2Type).c_str());
  if (Player2Type == PlayerType::Robot) {
    printf(" - %s", RobotConfig::GetRobotName(Robot2Type).c_str());
  }
  printf("\n\n");

  Ptr<MainWindow> mainWindow = new MainWindow(Player1Type, Player2Type, Robot1Type, Robot2Type);
  mainWindow->show();

  return Application.exec();
}

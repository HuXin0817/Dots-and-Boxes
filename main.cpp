#include <QStyleFactory>

#include "frontend/MainWindow.hpp"

int
main(int argc, char* argv[]) {
  QApplication Application(argc, argv);

  Application.setApplicationName("Dots and Boxes");
  Application.setApplicationVersion("1.0");
  Application.setOrganizationName("Dots and Boxes");
  Application.setStyle(QStyleFactory::create("Fusion"));

  auto Robot1Type = RobotType::L4;
  auto Robot2Type = RobotType::L4;
  auto Player1Type = PlayerType::Robot;
  auto Player2Type = PlayerType::Robot;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];

    if (arg == "--help" || arg == "-h") {
      printf("Usage: %s [OPTIONS]\n\n", argv[0]);
      printf("Options:\n");
      printf("  --player1=TYPE     Set player 1 type (human or robot, default: robot)\n");
      printf("  --player2=TYPE     Set player 2 type (human or robot, default: robot)\n");
      printf("  --robot1=LEVEL     Set robot 1 level (default: L4)\n");
      printf("  --robot2=LEVEL     Set robot 2 level (default: L4)\n");
      printf("  --help, -h         Show this help message and exit\n\n");
      printf("Robot Levels:\n");
      printf("  L0                 %s\n", RobotConfig::GetRobotName(RobotType::L0).c_str());
      printf("  L1                 %s\n", RobotConfig::GetRobotName(RobotType::L1).c_str());
      printf("  L2                 %s\n", RobotConfig::GetRobotName(RobotType::L2).c_str());
      printf("  L3                 %s\n", RobotConfig::GetRobotName(RobotType::L3).c_str());
      printf("  L4                 %s\n", RobotConfig::GetRobotName(RobotType::L4).c_str());
      return 0;
    }
    if (arg.find("--player1=") == 0) {
      std::string value = arg.substr(10);
      if (value == "human" || value == "Human" || value == "HUMAN") {
        Player1Type = PlayerType::Human;
      } else if (value == "robot" || value == "Robot" || value == "ROBOT") {
        Player1Type = PlayerType::Robot;
      }
    } else if (arg.find("--player2=") == 0) {
      std::string value = arg.substr(10);
      if (value == "human" || value == "Human" || value == "HUMAN") {
        Player2Type = PlayerType::Human;
      } else if (value == "robot" || value == "Robot" || value == "ROBOT") {
        Player2Type = PlayerType::Robot;
      }
    } else if (arg.find("--robot1=") == 0) {
      std::string value = arg.substr(9);
      Robot1Type = RobotConfig::ParseRobotType(value);
    } else if (arg.find("--robot2=") == 0) {
      std::string value = arg.substr(9);
      Robot2Type = RobotConfig::ParseRobotType(value);
    }
  }

  printf("Starting game with player configuration:\n");
  printf("  Player 1: %s", PlayerTypeToString(Player1Type).c_str());
  if (Player1Type == PlayerType::Robot) {
    printf(" - %s", RobotConfig::GetRobotName(Robot1Type).c_str());
  }
  printf("\n");
  printf("  Player 2: %s", PlayerTypeToString(Player2Type).c_str());
  if (Player2Type == PlayerType::Robot) {
    printf(" - %s", RobotConfig::GetRobotName(Robot2Type).c_str());
  }
  printf("\n\n");

  Ptr<MainWindow> mainWindow = new MainWindow(Player1Type, Player2Type, Robot1Type, Robot2Type);
  mainWindow->show();

  return Application.exec();
}
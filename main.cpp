#include "frontend/MainWindow.hpp"

static constexpr int BoardSize = 6;
static constexpr RobotType Robot1Type = RobotType::L4;
static constexpr RobotType Robot2Type = RobotType::L4;
static constexpr PlayerType Player1Type = PlayerType::Robot;
static constexpr PlayerType Player2Type = PlayerType::Robot;

namespace detail {
template <int N>
QWidget*
CreateMainWindowImpl(int size) {
  if (size == N) {
    return new MainWindow<N>(Player1Type, Player2Type, Robot1Type, Robot2Type);
  }
  if constexpr (N > 2) {
    return CreateMainWindowImpl<N - 1>(size);
  }
  return nullptr;
}
}  // namespace detail

QWidget*
CreateWindow(int BoardSize) {
  static constexpr int MaxBoardSize = 36;
  if (BoardSize <= 1 || BoardSize > MaxBoardSize) {
    return nullptr;
  }
  return detail::CreateMainWindowImpl<MaxBoardSize>(BoardSize);
}

int
main(int argc, char* argv[]) {
  QApplication Application(argc, argv);

  Application.setApplicationName("Dots and Boxes");
  Application.setApplicationVersion("1.0");
  Application.setOrganizationName("Dots and Boxes");

  printf("Starting game with player configuration:");
  printf("\n  Player 1: %s", PlayerConfig::GetPlayerTypeString(Player1Type).c_str());
  printf("\n  Player 2: %s", PlayerConfig::GetPlayerTypeString(Player2Type).c_str());
  printf("\n\n");

  QWidget* mainWindow = CreateWindow(BoardSize);
  if (mainWindow) {
    mainWindow->show();
  }

  return Application.exec();
}

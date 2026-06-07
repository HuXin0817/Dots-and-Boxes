#include "Frontend.h"

#include "../PlayerType.h"
#include "MainWindow.h"

namespace dab {

using namespace __detail__;

template <>
QMainWindow* CreateMainWindowImpl<BoardSize>(PlayerType player1Type, PlayerType player2Type) {
  return new frontend::MainWindow(player1Type, player2Type);
}

}  // namespace dab
#include "Frontend.h"

#include "../src/PlayerType.h"
#include "MainWindow.h"

namespace dab {

using namespace __detail__;

template <>
QWidget* CreateMainWindowImpl<BoardSize>(PlayerType player1Type, PlayerType player2Type) {
  return new __detail__::frontend::MainWindow(player1Type, player2Type);
}

}  // namespace dab
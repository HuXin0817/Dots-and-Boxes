#include "GlobalEnv.h"

namespace dab::__detail__::frontend {

void GlobalEnv::AddUnitSize() { UnitSize = std::max<int>(UnitSize + 1, static_cast<double>(UnitSize) * 1.1); }

void GlobalEnv::ReduceUnitSize() {
  UnitSize = std::min<int>(UnitSize - 1, static_cast<double>(UnitSize) * 0.9);
  if (UnitSize <= 0) {
    UnitSize = 1;
    QApplication::beep();
  }
}

void GlobalEnv::SetHumanMoveEdge(Edge edge) {
  if (Board.Contains(edge)) {
    return;
  }
  if (PlayerTypeIsRobot(Player1Type) && Board.IsPlayer1Turn()) {
    return;
  }
  if (PlayerTypeIsRobot(Player2Type) && Board.IsPlayer2Turn()) {
    return;
  }
  if (!HumanMoveEdgeQueue.try_push(edge)) {
    QApplication::beep();
  }
}

Edge GlobalEnv::GetHumanMoveEdgeSync() {
  Edge result;
  HumanMoveEdgeQueue.pop(result);
  return result;
}

}  // namespace dab::__detail__::frontend
#include "Env.h"

namespace dab::__detail__::frontend {

void Env::AddUnitSize() { UnitSize = std::max<int>(UnitSize + 1, static_cast<double>(UnitSize) * 1.1); }

void Env::ReduceUnitSize() {
  UnitSize = std::min<int>(UnitSize - 1, static_cast<double>(UnitSize) * 0.9);
  if (UnitSize <= 0) {
    UnitSize = 1;
    QApplication::beep();
  }
}

void Env::SetHumanMoveEdge(model::Edge edge) {
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

model::Edge Env::GetHumanMoveEdgeSync() {
  model::Edge result;
  HumanMoveEdgeQueue.pop(result);
  return result;
}

}  // namespace dab::__detail__::frontend
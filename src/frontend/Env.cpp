#include "Env.h"

#include "Color.h"

namespace dab::__detail__::frontend {

static constexpr QColor ToQColor(Color color) { return {color.r, color.g, color.b, color.a}; }

QColor Env::BackgroundColor() const {
  static constexpr iterable::Array<Color, 2> BackgroundColorMap = CreateBackgroundColorMap();
  return ToQColor(BackgroundColorMap.At(IsDarkTheme()));
}

QColor Env::DotColor() const {
  static constexpr iterable::Array<Color, 2> DotColorMap = CreateDotColorMap();
  return ToQColor(DotColorMap.At(IsDarkTheme()));
}

QColor Env::BoxColor(board::Owner owner) const {
  static constexpr iterable::Array<iterable::Array<Color, 3>, 2> BoxColorMap = CreateBoxColorMap();
  return ToQColor(BoxColorMap.At(IsDarkTheme()).At(static_cast<Int>(owner)));
}

QColor Env::EdgeColor(bool hovered, bool highlight, board::Owner owner) const {
  static constexpr iterable::Array<iterable::Array<iterable::Array<iterable::Array<Color, 3>, 2>, 2>, 2> EdgeColorMap =
      CreateEdgeColorMap();
  return ToQColor(EdgeColorMap.At(IsDarkTheme()).At(hovered).At(highlight).At(static_cast<Int>(owner)));
}

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
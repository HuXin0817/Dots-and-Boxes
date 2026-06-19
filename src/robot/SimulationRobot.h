#pragma once

#include "../Board.h"
#include "ImproveGreedyRobot.h"

namespace dab::__detail__::robot {

class SimulationRobot {
 public:
  SimulationRobot() = default;
  template <typename Board>
  iterable::Span<const model::Edge> BestCandidateEdges(const Board& board);
  iterable::List<model::Edge, model::Edge::Max>& GetSearchEdges() { return SearchEdges; }

 private:
  ImproveGreedyRobot SubRobot;
  board::RelativeScoreBoard SimulationBoard;
  iterable::List<model::Edge, model::Edge::Max> SearchEdges;
};

}  // namespace dab::__detail__::robot
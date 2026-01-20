#pragma once

#include "../board/ScoreCountableBoard.hpp"
#include "../common/Span.hpp"
#include "../model/Edge.hpp"

namespace dab::robot {

template <int BoardSize>
class Robot {
  public:
  virtual ~Robot() = default;

  virtual common::Span<model::Edge<BoardSize>>
  BestCandidateEdges(const board::ScoreCountableBoard<BoardSize>& board) = 0;
};

}  // namespace dab::robot

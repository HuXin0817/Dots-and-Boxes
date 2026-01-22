#pragma once

#include "../board/ScoreCountableBoard.hpp"
#include "../common/Span.hpp"
#include "../model/Edge.hpp"

namespace dab::robot {

template <int BoardSize, typename SizeType>
class Robot {
  public:
  virtual ~Robot() = default;

  virtual common::Span<model::Edge<BoardSize, SizeType>, int>
  BestCandidateEdges(const board::ScoreCountableBoard<BoardSize, SizeType>& board) = 0;
};

}  // namespace dab::robot

#pragma once

#include "../board/ScoreCountableBoard.hpp"
#include "../common/Span.hpp"
#include "../model/Edge.hpp"

class Robot {
  public:
  virtual ~Robot() = default;

  virtual Span<Edge>
  BestCandidateEdges(const ScoreCountableBoard& board) = 0;
};

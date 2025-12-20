#pragma once

#include "../board/ScoreCountableBoard.hpp"
#include "../common/Span.hpp"
#include "../model/Edge.hpp"

template <int BoardSize>
class Robot {
  public:
  virtual ~Robot() = default;

  virtual Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) = 0;
};

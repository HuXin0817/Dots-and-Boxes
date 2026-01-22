#pragma once

#include "../board/ScoreCountableBoard.hpp"
#include "../common/Span.hpp"
#include "../model/Edge.hpp"

template <int BoardSize, typename SizeType>
class Robot {
  public:
  virtual ~Robot() = default;

  virtual Span<Edge<BoardSize, SizeType>, SizeType>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize, SizeType>& board) = 0;
};

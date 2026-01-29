#pragma once

#include "Board.hpp"
#include "Common.hpp"
#include "Model.hpp"

namespace dab::detail::robot {

template <int64_t BoardSize>
class Robot {
  public:
  virtual ~Robot() = default;

  virtual Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) = 0;
};

}  // namespace dab::detail::robot
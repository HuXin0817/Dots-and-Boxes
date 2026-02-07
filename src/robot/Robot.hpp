#pragma once

#include <Dab/Board.hpp>

namespace dab::detail::robot {

template <int64_t BoardSize>
class Robot {
 public:
  virtual ~Robot() = default;

  virtual Span<Edge<BoardSize>>
  BestCandidateEdges(const RelativeScoreBoard<BoardSize>& board) = 0;
};

}  // namespace dab::detail::robot
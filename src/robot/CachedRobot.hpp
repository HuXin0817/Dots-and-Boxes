#pragma once

#include <cstddef>
#include <vector>

#include "Board.hpp"
#include "Common.hpp"
#include "Robot.hpp"

namespace dab::detail::robot {

template <int64_t BoardSize, typename SubRobotType>
class CachedRobot : public Robot<BoardSize> {
  public:
  CachedRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override;

  private:
  SubRobotType SubRobot;

  static inline LRUCache<HashBoard<BoardSize>, std::vector<Edge<BoardSize>>> GlobalCache =
      LRUCache<HashBoard<BoardSize>, std::vector<Edge<BoardSize>>>(1 << 14);
};

template <int64_t BoardSize, typename SubRobotType>
Span<Edge<BoardSize>>
CachedRobot<BoardSize, SubRobotType>::BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) {
  typename dab::LRUCache<HashBoard<BoardSize>, std::vector<Edge<BoardSize>>>::ConstAccessor ac;
  if (GlobalCache.Find(ac, board)) {
    return Span(ac->data(), ac->data() + ac->size());
  }

  Span<Edge<BoardSize>> result = SubRobot.BestCandidateEdges(board);
  GlobalCache.Insert(board, std::vector<Edge<BoardSize>>(result.Begin(), result.End()));
  return result;
}

}  // namespace dab::detail::robot
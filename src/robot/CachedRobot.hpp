#pragma once

#include <Dab/Board.hpp>
#include <cstddef>
#include <cstdint>

#include "Robot.hpp"

namespace dab::detail::robot {

template <int64_t BoardSize, typename SubRobotType>
class CachedRobot : public Robot<BoardSize> {
  static constexpr uint32_t Cap = Edge<BoardSize>::Max << 8;

  public:
  CachedRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const RelativeScoreBoard<BoardSize>& board) override;

  private:
  SubRobotType SubRobot;

  static inline LRUCache<HashBoard<BoardSize>, Vector<Edge<BoardSize>>, Cap> GlobalCache;
};

template <int64_t BoardSize, typename SubRobotType>
Span<Edge<BoardSize>>
CachedRobot<BoardSize, SubRobotType>::BestCandidateEdges(const RelativeScoreBoard<BoardSize>& board) {
  if (typename LRUCache<HashBoard<BoardSize>, Vector<Edge<BoardSize>>, Cap>::ConstAccessor accessor;
      GlobalCache.Find(accessor, board)) {
    return Span(accessor->begin(), accessor->end());
  }

  Span<Edge<BoardSize>> result = SubRobot.BestCandidateEdges(board);
  GlobalCache.Insert(board, Vector<Edge<BoardSize>>(result.begin(), result.end()));
  return result;
}

}  // namespace dab::detail::robot
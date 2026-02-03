#pragma once

#include <cstddef>

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

  static inline LRUCache<HashBoard<BoardSize>, Vector<Edge<BoardSize>>> GlobalCache =
      LRUCache<HashBoard<BoardSize>, Vector<Edge<BoardSize>>>(1 << 14);
};

template <int64_t BoardSize, typename SubRobotType>
Span<Edge<BoardSize>>
CachedRobot<BoardSize, SubRobotType>::BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) {
  if (typename dab::LRUCache<HashBoard<BoardSize>, Vector<Edge<BoardSize>>>::ConstAccessor ac;
      GlobalCache.Find(ac, board)) {
    return Span(ac->begin(), ac->end());
  }

  Span<Edge<BoardSize>> result = SubRobot.BestCandidateEdges(board);
  GlobalCache.Insert(board, Vector<Edge<BoardSize>>(result.begin(), result.end()));
  return result;
}

}  // namespace dab::detail::robot
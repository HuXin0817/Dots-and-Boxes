#pragma once

#include <Dab/Board.hpp>
#include <cstddef>

#include "Robot.hpp"

namespace dab::detail::robot {

template <int64_t BoardSize, typename SubRobotType, uint32_t Cap>
class CachedRobot : public Robot<BoardSize> {
  public:
  CachedRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override;

  private:
  SubRobotType SubRobot;

  static inline LRUCache<HashBoard<BoardSize>, Vector<Edge<BoardSize>>, Cap> GlobalCache;
};

template <int64_t BoardSize, typename SubRobotType, uint32_t Cap>
Span<Edge<BoardSize>>
CachedRobot<BoardSize, SubRobotType, Cap>::BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) {
  if (typename dab::LRUCache<HashBoard<BoardSize>, Vector<Edge<BoardSize>>, Cap>::ConstAccessor ac;
      GlobalCache.Find(ac, board)) {
    return Span(ac->begin(), ac->end());
  }

  Span<Edge<BoardSize>> result = SubRobot.BestCandidateEdges(board);
  GlobalCache.Insert(board, Vector<Edge<BoardSize>>(result.begin(), result.end()));
  return result;
}

}  // namespace dab::detail::robot
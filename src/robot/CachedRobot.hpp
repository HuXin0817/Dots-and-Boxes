#pragma once

#include <cstddef>
#include <cstdio>
#include <vector>

#include "Board.hpp"
#include "Common.hpp"
#include "Robot.hpp"

namespace dab::detail::robot {

template <int64_t BoardSize, typename SubRobotType, size_t HashSize = 1 << 8>
class CachedRobot : public Robot<BoardSize> {
  public:
  CachedRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override;

  private:
  SubRobotType SubRobot;
  LRUCache<HashBoard<BoardSize>, std::vector<Edge<BoardSize>>, HashSize> Cache;
};

template <int64_t BoardSize, typename SubRobotType, size_t HashSize>
Span<Edge<BoardSize>>
CachedRobot<BoardSize, SubRobotType, HashSize>::BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) {
  if (std::vector<Edge<BoardSize>>* cached = Cache.Get(board)) {
    return Span<Edge<BoardSize>>(cached->data(), cached->data() + cached->size());
  }

  Span<Edge<BoardSize>> result = SubRobot.BestCandidateEdges(board);
  Cache.Put(board, std::vector(result.Begin(), result.End()));
  return result;
}

}  // namespace dab::detail::robot
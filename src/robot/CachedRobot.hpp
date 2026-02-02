#pragma once

#include <cstddef>
#include <cstdio>
#include <unordered_map>
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
  std::unordered_map<HashBoard<BoardSize>, std::vector<Edge<BoardSize>>> Cache;
};

template <int64_t BoardSize, typename SubRobotType, size_t HashSize>
Span<Edge<BoardSize>>
CachedRobot<BoardSize, SubRobotType, HashSize>::BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) {
  if (auto it = Cache.find(board); it != Cache.end()) {
    return Span<Edge<BoardSize>>(it->second.data(), it->second.data() + it->second.size());
  }

  Span<Edge<BoardSize>> result = SubRobot.BestCandidateEdges(board);
  Cache[board] = std::vector(result.Begin(), result.End());
  return result;
}

}  // namespace dab::detail::robot
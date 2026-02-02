#pragma once

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <cstdio>
#include <mutex>
#include <shared_mutex>
#include <utility>
#include <vector>

#include "Board.hpp"
#include "Common.hpp"
#include "Robot.hpp"

namespace dab::detail::robot {

template <int64_t BoardSize, typename SubRobotType, size_t HashSize = 1 << 12>
class CachedRobot : public Robot<BoardSize> {
  public:
  CachedRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override;

  private:
  SubRobotType SubRobot;

  static bool
  IsEqual(const BasicBoard<BoardSize>& board1, const BasicBoard<BoardSize>& board2);

  static inline std::atomic<int> CachedCount = 0;
  static inline std::atomic<int> TotalCount = 0;

  static Array<std::vector<std::pair<BasicBoard<BoardSize>, std::vector<Edge<BoardSize>>>>, HashSize>
  CreateCache();
  static inline Array<std::vector<std::pair<BasicBoard<BoardSize>, std::vector<Edge<BoardSize>>>>, HashSize>
      GlobalCache = CreateCache();
  static inline Array<std::shared_mutex, HashSize> GlobalCacheMutex;
};

template <int64_t BoardSize, typename SubRobotType, size_t HashSize>
Span<Edge<BoardSize>>
CachedRobot<BoardSize, SubRobotType, HashSize>::BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) {
  TotalCount++;
  if (TotalCount % 100000 == 0) {
    size_t MaxSize = 0;
    for (size_t i = 0; i < HashSize; i++) {
      MaxSize = std::max(MaxSize, GlobalCache[i].size());
    }

    printf("[CachedRobot] Cached:Total %d:%d MaxSize %lu\n", CachedCount.load(), TotalCount.load(), MaxSize);
  }

  size_t HashValue = board.Hash();
  size_t HashOffset = HashValue % HashSize;

  std::vector<std::pair<BasicBoard<BoardSize>, std::vector<Edge<BoardSize>>>>& CacheList = GlobalCache[HashOffset];
  for (const std::pair<BasicBoard<BoardSize>, std::vector<Edge<BoardSize>>>& entry : CacheList) {
    if (IsEqual(entry.first, static_cast<BasicBoard<BoardSize>>(board))) {
      CachedCount++;
      return Span(entry.second.data(), entry.second.data() + entry.second.size());
    }
  }

  Span<Edge<BoardSize>> result = SubRobot.BestCandidateEdges(board);
  std::vector<Edge<BoardSize>> resultDump(result.begin(), result.end());

  std::unique_lock lock(GlobalCacheMutex[HashOffset]);
  CacheList.emplace_back(static_cast<BasicBoard<BoardSize>>(board), resultDump);
  return result;
}

template <int64_t BoardSize, typename SubRobotType, size_t HashSize>
bool
CachedRobot<BoardSize, SubRobotType, HashSize>::IsEqual(const BasicBoard<BoardSize>& board1,
                                                        const BasicBoard<BoardSize>& board2) {
  if (board1.NowStep() != board2.NowStep()) {
    return false;
  }
  for (Edge<BoardSize> edge : board2.MoveRecord()) {
    if (!board1.Contains(edge)) {
      return false;
    }
  }
  return true;
}

template <int64_t BoardSize, typename SubRobotType, size_t HashSize>
Array<std::vector<std::pair<BasicBoard<BoardSize>, std::vector<Edge<BoardSize>>>>, HashSize>
CachedRobot<BoardSize, SubRobotType, HashSize>::CreateCache() {
  Array<std::vector<std::pair<BasicBoard<BoardSize>, std::vector<Edge<BoardSize>>>>, HashSize> Cache;
  for (std::vector<std::pair<BasicBoard<BoardSize>, std::vector<Edge<BoardSize>>>>& arr : Cache) {
    arr.reserve(Edge<BoardSize>::Max);
  }
  return Cache;
}

}  // namespace dab::detail::robot
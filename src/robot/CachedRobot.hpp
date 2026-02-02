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

template <int64_t BoardSize, typename SubRobotType, size_t Hash1Size = 64, size_t Hash2Size = 64>
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

  static Array<Array<std::vector<std::pair<BasicBoard<BoardSize>, std::vector<Edge<BoardSize>>>>, Hash2Size>, Hash1Size>
  CreateCache();
  static inline Array<Array<std::vector<std::pair<BasicBoard<BoardSize>, std::vector<Edge<BoardSize>>>>, Hash2Size>,
                      Hash1Size>
      GlobalCache = CreateCache();
  static inline Array<Array<std::shared_mutex, Hash2Size>, Hash1Size> GlobalCacheMutex;
};

template <int64_t BoardSize, typename SubRobotType, size_t Hash1Size, size_t Hash2Size>
Span<Edge<BoardSize>>
CachedRobot<BoardSize, SubRobotType, Hash1Size, Hash2Size>::BestCandidateEdges(
    const ScoreCountableBoard<BoardSize>& board) {
  TotalCount++;
  if (TotalCount % 100000 == 0) {
    size_t MaxSize = 0;
    for (size_t i = 0; i < Hash1Size; i++) {
      for (size_t j = 0; j < Hash2Size; j++) {
        MaxSize = std::max(MaxSize, GlobalCache[i][j].size());
      }
    }

    printf("[CachedRobot] Cached:Total %d:%d MaxSize %lu\n", CachedCount.load(), TotalCount.load(), MaxSize);
  }

  size_t HashValue1 = board.Hash1();
  size_t HashValue2 = board.Hash2();
  size_t Hash1Offset = HashValue1 % Hash1Size;
  size_t Hash2Offset = HashValue2 % Hash2Size;

  std::vector<std::pair<BasicBoard<BoardSize>, std::vector<Edge<BoardSize>>>>& CacheList =
      GlobalCache[Hash1Offset][Hash2Offset];
  for (const std::pair<BasicBoard<BoardSize>, std::vector<Edge<BoardSize>>>& entry : CacheList) {
    if (IsEqual(entry.first, static_cast<BasicBoard<BoardSize>>(board))) {
      CachedCount++;
      return Span(entry.second.data(), entry.second.data() + entry.second.size());
    }
  }

  Span<Edge<BoardSize>> result = SubRobot.BestCandidateEdges(board);
  std::vector<Edge<BoardSize>> resultDump(result.begin(), result.end());

  std::unique_lock lock(GlobalCacheMutex[Hash1Offset][Hash2Offset]);
  CacheList.emplace_back(static_cast<BasicBoard<BoardSize>>(board), resultDump);
  return result;
}

template <int64_t BoardSize, typename SubRobotType, size_t Hash1Size, size_t Hash2Size>
bool
CachedRobot<BoardSize, SubRobotType, Hash1Size, Hash2Size>::IsEqual(const BasicBoard<BoardSize>& board1,
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

template <int64_t BoardSize, typename SubRobotType, size_t Hash1Size, size_t Hash2Size>
Array<Array<std::vector<std::pair<BasicBoard<BoardSize>, std::vector<Edge<BoardSize>>>>, Hash2Size>, Hash1Size>
CachedRobot<BoardSize, SubRobotType, Hash1Size, Hash2Size>::CreateCache() {
  Array<Array<std::vector<std::pair<BasicBoard<BoardSize>, std::vector<Edge<BoardSize>>>>, Hash2Size>, Hash1Size> Cache;
  for (Array<std::vector<std::pair<BasicBoard<BoardSize>, std::vector<Edge<BoardSize>>>>, Hash2Size>& arr1 : Cache) {
    for (std::vector<std::pair<BasicBoard<BoardSize>, std::vector<Edge<BoardSize>>>>& arr2 : arr1) {
      arr2.reserve(Edge<BoardSize>::Max);
    }
  }
  return Cache;
}

}  // namespace dab::detail::robot
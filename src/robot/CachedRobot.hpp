#pragma once

#include <cstddef>
#include <cstdio>
#include <mutex>
#include <vector>

#include "Board.hpp"
#include "Common.hpp"
#include "Robot.hpp"

namespace dab::detail::robot {

template <int64_t BoardSize>
class RobotCacheEntry {
  public:
  RobotCacheEntry() = default;
  RobotCacheEntry(const BasicBoard<BoardSize>& board, const std::vector<Edge<BoardSize>>& result)
      : Board(board), Result(result) {
  }

  BasicBoard<BoardSize> Board;
  std::vector<Edge<BoardSize>> Result;
};

template <int64_t BoardSize>
class RobotCache {
  public:
  RobotCache() {
    Entrise.reserve(Edge<BoardSize>::Max);
  }

  void
  Find(const BasicBoard<BoardSize>& board, Span<Edge<BoardSize>>& result) {
    for (size_t i = 0; i < Entrise.size(); i++) {
      RobotCacheEntry<BoardSize>& entry = Entrise[i];
      if (entry.Board == board) {
        result = Span(entry.Result.data(), entry.Result.data() + entry.Result.size());
      }
    }
  }

  void
  Add(const BasicBoard<BoardSize>& board, const Span<Edge<BoardSize>>& result) {
    std::vector<Edge<BoardSize>> resultDump(result.Begin(), result.End());
    std::lock_guard lock(Mutex);
    Entrise.emplace_back(board, resultDump);
  }

  private:
  std::mutex Mutex;
  std::vector<RobotCacheEntry<BoardSize>> Entrise;
};

template <int64_t BoardSize, typename SubRobotType, size_t HashSize = 1 << 14>
class CachedRobot : public Robot<BoardSize> {
  public:
  CachedRobot() = default;

  Span<Edge<BoardSize>>
  BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) override;

  private:
  SubRobotType SubRobot;

  static inline std::vector<RobotCache<BoardSize>> GlobalCache = std::vector<RobotCache<BoardSize>>(HashSize);
};

template <int64_t BoardSize, typename SubRobotType, size_t HashSize>
Span<Edge<BoardSize>>
CachedRobot<BoardSize, SubRobotType, HashSize>::BestCandidateEdges(const ScoreCountableBoard<BoardSize>& board) {
  RobotCache<BoardSize>& CacheList = GlobalCache[board.Hash() % HashSize];

  Span<Edge<BoardSize>> result;
  CacheList.Find(board, result);
  if (result.Empty()) {
    result = SubRobot.BestCandidateEdges(board);
    CacheList.Add(board, result);
  }

  return result;
}

}  // namespace dab::detail::robot
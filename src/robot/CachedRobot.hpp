#pragma once

#include <cstddef>
#include <mutex>
#include <vector>
#include <list>
#include <unordered_map>

#include "Board.hpp"
#include "Common.hpp"
#include "Robot.hpp"

namespace dab::detail::robot {

template <int64_t BoardSize>
class RobotCacheEntry {
  public:
  RobotCacheEntry() = default;
  RobotCacheEntry(const std::vector<Edge<BoardSize>>& result)
      : Result(result) {
  }

  std::vector<Edge<BoardSize>> Result;
  size_t Hash;
};

template <int64_t BoardSize, size_t Cap = 1 << 10>
class RobotCache {
  public:
  RobotCache() = default;

  void
  Find(size_t hash, Span<Edge<BoardSize>>& result) {
    std::lock_guard lock(Mutex);
    auto it = CacheMap.find(hash);
    if (it != CacheMap.end()) {
      LRUList.splice(LRUList.begin(), LRUList, it->second);
      result = Span(it->second->Result.data(), it->second->Result.data() + it->second->Result.size());
    }
  }

  void
  Add(size_t hash, const Span<Edge<BoardSize>>& result) {
    std::vector<Edge<BoardSize>> resultDump(result.Begin(), result.End());
    std::lock_guard lock(Mutex);
    
    auto it = CacheMap.find(hash);
    if (it != CacheMap.end()) {
      LRUList.splice(LRUList.begin(), LRUList, it->second);
      it->second->Result = resultDump;
    } else {
      if (LRUList.size() >= Cap) {
        CacheMap.erase(LRUList.back().Hash);
        LRUList.pop_back();
      }
      LRUList.emplace_front(resultDump);
      LRUList.front().Hash = hash;
      CacheMap[hash] = LRUList.begin();
    }
  }

  private:
  std::mutex Mutex;
  std::list<RobotCacheEntry<BoardSize>> LRUList;
  std::unordered_map<size_t, typename std::list<RobotCacheEntry<BoardSize>>::iterator> CacheMap;
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
  size_t hash = board.Hash();
  RobotCache<BoardSize>& CacheList = GlobalCache[hash % HashSize];

  Span<Edge<BoardSize>> result;
  CacheList.Find(hash, result);
  if (result.Empty()) {
    result = SubRobot.BestCandidateEdges(board);
    CacheList.Add(hash, result);
  }

  return result;
}

}  // namespace dab::detail::robot
#pragma once

#include <thread-safe-lru/lru-cache.h>

#include "../Board.h"

namespace dab::__detail__::robot {

template <typename SubRobotType>
class CachedRobot : public SubRobotType {
 public:
  CachedRobot() = default;

  template <typename Board>
  Span<const Edge> BestCandidateEdges(const Board& board);

 private:
  using CacheType = tstarling::ThreadSafeLRUCache<HashValueBoard, Vector<Edge>>;
  static inline CacheType Cache{static_cast<std::size_t>(Edge::Max) << 10};

  HashValueBoard Key;
};

template <typename SubRobotType>
template <typename Board>
Span<const Edge> CachedRobot<SubRobotType>::BestCandidateEdges(const Board& board) {
  Key = board;
  if (CacheType::ConstAccessor ac; Cache.find(ac, Key)) {
    return *ac;
  }

  const Span result = SubRobotType::BestCandidateEdges(board);
  assert(!result.Empty());
  Cache.insert(Key, result);
  return result;
}

}  // namespace dab::__detail__::robot
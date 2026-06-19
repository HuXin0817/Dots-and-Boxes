#pragma once

#include <thread-safe-lru/lru-cache.h>

#include "../Board.h"

namespace dab::__detail__::robot {

template <typename SubRobotType>
class CachedRobot : public SubRobotType {
 public:
  CachedRobot() = default;

  template <typename Board>
  iterable::Span<const model::Edge> BestCandidateEdges(const Board& board);

 private:
  using CacheType = tstarling::ThreadSafeLRUCache<board::HashValueBoard, iterable::Vector<model::Edge>>;
  static inline CacheType Cache = CacheType(static_cast<std::size_t>(model::Edge::Max) << 10);

  board::HashValueBoard Key;
};

template <typename SubRobotType>
template <typename Board>
iterable::Span<const model::Edge> CachedRobot<SubRobotType>::BestCandidateEdges(const Board& board) {
  Key = board;
  if (CacheType::ConstAccessor ac; Cache.find(ac, Key)) {
    return *ac;
  }

  const iterable::Span<const model::Edge> result = SubRobotType::BestCandidateEdges(board);
  assert(!result.Empty());
  Cache.insert(Key, result);
  return result;
}

}  // namespace dab::__detail__::robot
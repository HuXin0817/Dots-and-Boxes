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
  static constexpr std::size_t CacheSize = static_cast<std::size_t>(model::Edge::Max) << 10;
  static inline CacheType Cache = CacheType(CacheSize);
};

template <typename SubRobotType>
template <typename Board>
iterable::Span<const model::Edge> CachedRobot<SubRobotType>::BestCandidateEdges(const Board& board) {
  board::HashValueBoard key = board;
  if (CacheType::ConstAccessor ac; Cache.find(ac, key)) {
    return *ac;
  }

  const iterable::Span<const model::Edge> value = SubRobotType::BestCandidateEdges(board);
  assert(!result.Empty());
  Cache.insert(key, value);
  return value;
}

}  // namespace dab::__detail__::robot
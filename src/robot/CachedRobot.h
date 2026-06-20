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
  using Cache = tstarling::ThreadSafeLRUCache<board::HashValueBoard, iterable::Vector<model::Edge>>;
  static constexpr std::size_t CacheSize = static_cast<std::size_t>(model::Edge::Max) << 10;
  static inline Cache Map = Cache(CacheSize);
};

template <typename SubRobotType>
template <typename Board>
iterable::Span<const model::Edge> CachedRobot<SubRobotType>::BestCandidateEdges(const Board& board) {
  board::HashValueBoard key = board;
  if (Cache::ConstAccessor ac; Map.find(ac, key)) {
    return *ac;
  }

  const iterable::Span<const model::Edge> value = SubRobotType::BestCandidateEdges(board);
  assert(!result.Empty());
  Map.insert(key, value);
  return value;
}

}  // namespace dab::__detail__::robot
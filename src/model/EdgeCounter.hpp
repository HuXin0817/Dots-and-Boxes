#pragma once

#include <Dab/Common.hpp>
#include <algorithm>
#include <cassert>
#include <cstdint>

#include "NearBoxes.hpp"
#include "Square.hpp"

namespace dab::detail::model {

template <int64_t BoardSize>
class EdgeCounter {
  public:
  EdgeCounter();

  void
  Reset();
  Int<BoardSize>
  Add(const Edge<BoardSize> edge);
  uint8_t
  EdgeCount(const Box<BoardSize> box) const;
  uint8_t
  MaxEdgeCount(const Edge<BoardSize> edge) const;

  private:
  Array<uint8_t, Box<BoardSize>::Max> Counter;
};

template <int64_t BoardSize>
EdgeCounter<BoardSize>::EdgeCounter() {
  Reset();
}

template <int64_t BoardSize>
void
EdgeCounter<BoardSize>::Reset() {
  Counter = Array<uint8_t, Box<BoardSize>::Max>();
}

template <int64_t BoardSize>
Int<BoardSize>
EdgeCounter<BoardSize>::Add(const Edge<BoardSize> edge) {
  Int<BoardSize> score = 0;
  for (const Box<BoardSize> box : NearBoxes(edge)) {
    Counter[box]++;
    const uint8_t num = Counter[box];
    assert(num <= 4);
    if (num == 4) {
      score++;
    }
  }
  return score;
}

template <int64_t BoardSize>
uint8_t
EdgeCounter<BoardSize>::EdgeCount(const Box<BoardSize> box) const {
  return Counter[box];
}

template <int64_t BoardSize>
uint8_t
EdgeCounter<BoardSize>::MaxEdgeCount(const Edge<BoardSize> edge) const {
  uint8_t maxCount = 0;
  for (const Box<BoardSize> box : NearBoxes(edge)) {
    maxCount = std::max(maxCount, EdgeCount(box));
  }
  return maxCount;
}

}  // namespace dab::detail::model
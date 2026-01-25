#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>

#include "../common/Array.hpp"
#include "NearBoxes.hpp"
#include "Square.hpp"

template <int64_t BoardSize>
class EdgeCounter {
  public:
  EdgeCounter();

  void
  Reset();
  SizeType<BoardSize>
  Add(const Edge<BoardSize> edge);
  uint8_t
  EdgeCount(const Box<BoardSize> box) const;
  uint8_t
  MaxEdgeCount(const Edge<BoardSize> edge) const;

  private:
  Array<uint8_t, Box<BoardSize>::Max> Map;
};

template <int64_t BoardSize>
EdgeCounter<BoardSize>::EdgeCounter() {
  Reset();
}

template <int64_t BoardSize>
void
EdgeCounter<BoardSize>::Reset() {
  Map = Array<uint8_t, Box<BoardSize>::Max>();
}

template <int64_t BoardSize>
SizeType<BoardSize>
EdgeCounter<BoardSize>::Add(const Edge<BoardSize> edge) {
  SizeType<BoardSize> score = 0;
  for (const Box<BoardSize> box : NearBoxes(edge)) {
    Map.At(box.Value())++;
    const uint8_t num = Map.At(box.Value());
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
  return Map.At(box.Value());
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

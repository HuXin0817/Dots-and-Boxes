#pragma once

#include <algorithm>

#include "../common/Array.hpp"
#include "NearBoxes.hpp"
#include "Square.hpp"

template <int64_t BoardSize>
class EdgeCountOfBox {
  public:
  EdgeCountOfBox() = default;

  Int<BoardSize>
  Add(const Edge<BoardSize> edge) {
    Int<BoardSize> score = 0;
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

  uint8_t
  EdgeCount(const Box<BoardSize> box) const {
    return Map.At(box.Value());
  }

  uint8_t
  MaxCount(const Edge<BoardSize> edge) const {
    uint8_t maxCount = 0;
    for (const Box<BoardSize> box : NearBoxes(edge)) {
      maxCount = std::max(maxCount, Map.At(box.Value()));
    }
    return maxCount;
  }

  private:
  Array<uint8_t, Box<BoardSize>::Max> Map;
};

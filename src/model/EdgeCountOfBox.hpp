#pragma once

#include <algorithm>

#include "../common/Array.hpp"
#include "NearBoxes.hpp"
#include "Square.hpp"

template <int BoardSize>
class EdgeCountOfBox {
  public:
  EdgeCountOfBox() = default;

  SizeType
  Add(const Edge<BoardSize> edge) {
    SizeType score = 0;
    for (const Box<BoardSize> box : NearBoxes(edge)) {
      Map.At(box.Value())++;
      const int8_t num = Map.At(box.Value());
      assert(num <= 4);
      if (num == 4) {
        score++;
      }
    }
    return score;
  }

  int8_t
  EdgeCount(const Box<BoardSize> box) const {
    return Map.At(box.Value());
  }

  int8_t
  MaxCount(const Edge<BoardSize> edge) const {
    int8_t maxCount = 0;
    for (const Box<BoardSize> box : NearBoxes(edge)) {
      maxCount = std::max(maxCount, Map.At(box.Value()));
    }
    return maxCount;
  }

  private:
  Array<int8_t, Box<BoardSize>::Max> Map;
};

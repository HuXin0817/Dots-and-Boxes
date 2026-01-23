#pragma once

#include "../common/Array.hpp"
#include "NearBoxes.hpp"
#include "Square.hpp"

template <int64_t BoardSize>
class EdgeCountOfBox {
  public:
  EdgeCountOfBox() = default;

  SizeType<BoardSize>
  Add(const Edge<BoardSize> edge) {
    SizeType<BoardSize> score = 0;
    for (const Box<BoardSize> box : NearBoxes(edge)) {
      Map.At(box.Value())++;
      const SizeType<BoardSize> num = Map.At(box.Value());
      assert(num <= 4);
      if (num == 4) {
        score++;
      }
    }
    return score;
  }

  SizeType<BoardSize>
  EdgeCount(const Box<BoardSize> box) const {
    return Map.At(box.Value());
  }

  SizeType<BoardSize>
  MaxCount(const Edge<BoardSize> edge) const {
    SizeType<BoardSize> maxCount = 0;
    for (const Box<BoardSize> box : NearBoxes(edge)) {
      maxCount = std::max(maxCount, Map.At(box.Value()));
    }
    return maxCount;
  }

  private:
  Array<SizeType<BoardSize>, Box<BoardSize>::Max, SizeType<BoardSize>> Map;
};

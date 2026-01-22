#pragma once

#include "../common/Array.hpp"
#include "NearBoxes.hpp"
#include "Square.hpp"

template <int BoardSize, typename SizeType>
class EdgeCountOfBox {
  public:
  EdgeCountOfBox() = default;

  SizeType
  Add(const Edge<BoardSize, SizeType> edge) {
    SizeType score = 0;
    for (const Box<BoardSize, SizeType> box : NearBoxes(edge)) {
      Map.At(box.Value())++;
      const SizeType num = Map.At(box.Value());
      assert(num <= 4);
      if (num == 4) {
        score++;
      }
    }
    return score;
  }

  SizeType
  EdgeCount(const Box<BoardSize, SizeType> box) const {
    return Map.At(box.Value());
  }

  SizeType
  MaxCount(const Edge<BoardSize, SizeType> edge) const {
    SizeType maxCount = 0;
    for (const Box<BoardSize, SizeType> box : NearBoxes(edge)) {
      maxCount = std::max(maxCount, Map.At(box.Value()));
    }
    return maxCount;
  }

  private:
  Array<SizeType, Box<BoardSize, SizeType>::Max, SizeType> Map;
};

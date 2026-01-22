#pragma once

#include <algorithm>
#include <cassert>

#include "../common/Array.hpp"
#include "NearBoxes.hpp"
#include "Square.hpp"

namespace dab::model {

template <int BoardSize, typename SizeType>
class EdgeCountOfBox {
  public:
  EdgeCountOfBox() = default;

  int
  Add(const Edge<BoardSize, SizeType> edge) {
    int score = 0;
    for (const Box<BoardSize, SizeType> box : NearBoxes(edge)) {
      ++Map.At(box.Value());
      const int num = Map.At(box.Value());
      assert(num <= 4);
      if (num == 4) {
        score++;
      }
    }
    return score;
  }

  int
  EdgeCount(const Box<BoardSize, SizeType> box) const {
    return Map.At(box.Value());
  }

  int
  MaxCount(const Edge<BoardSize, SizeType> edge) const {
    int maxCount = 0;
    for (const Box<BoardSize, SizeType> box : NearBoxes(edge)) {
      maxCount = std::max(maxCount, Map.At(box.Value()));
    }
    return maxCount;
  }

  private:
  common::Array<int, Box<BoardSize, SizeType>::Max, int> Map;
};

}  // namespace dab::model

#pragma once

#include <cassert>

#include "../common/Array.hpp"
#include "NearBoxes.hpp"

template <int BoardSize>
class EdgeCountOfBox {
  public:
  EdgeCountOfBox() = default;

  int
  Add(Edge<BoardSize> edge) {
    int score = 0;
    for (auto box : NearBoxes(edge)) {
      Map.At(box.Int())++;
      int num = Map.At(box.Int());
      assert(num <= 4);
      if (num == 4) {
        score++;
      }
    }
    return score;
  }

  int
  EdgeCount(Box<BoardSize> box) const {
    return Map.At(box.Int());
  }

  int
  MaxCount(Edge<BoardSize> edge) const {
    int maxCount = 0;
    for (auto box : NearBoxes(edge)) {
      maxCount = std::max(maxCount, Map.At(box.Int()));
    }
    return maxCount;
  }

  private:
  Array<int, Box<BoardSize>::Max> Map;
};

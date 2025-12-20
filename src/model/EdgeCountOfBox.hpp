#pragma once

#include <cassert>

#include "../common/Array.hpp"
#include "NearBoxes.hpp"

template <int BoardSize>
class EdgeCountOfBox : public Array<int, Box<BoardSize>::Max> {
  public:
  EdgeCountOfBox() = default;

  int
  Add(Edge<BoardSize> edge) {
    int score = 0;
    for (Box box : NearBoxes(edge)) {
      Array<int, Box<BoardSize>::Max>::At(box)++;
      int num = Array<int, Box<BoardSize>::Max>::At(box);
      assert(num <= 4);
      if (num == 4) {
        score++;
      }
    }
    return score;
  }

  int
  MaxCount(Edge<BoardSize> edge) const {
    int maxCount = 0;
    for (Box box : NearBoxes(edge)) {
      maxCount = std::max(maxCount, Array<int, Box<BoardSize>::Max>::At(box));
    }
    return maxCount;
  }
};

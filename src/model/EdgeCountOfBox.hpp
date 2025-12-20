#pragma once

#include "../common/Array.hpp"
#include "NearBoxes.hpp"

class EdgeCountOfBox : public Array<int, Box::Max> {
  public:
  EdgeCountOfBox() = default;

  int
  Add(Edge edge) {
    int score = 0;
    for (Box box : NearBoxes(edge)) {
      At(box)++;
      assert(At(box) <= 4);
      if (At(box) == 4) {
        score++;
      }
    }
    return score;
  }

  int
  MaxCount(Edge edge) const {
    int maxCount = 0;
    for (Box box : NearBoxes(edge)) {
      maxCount = std::max(maxCount, At(box));
    }
    return maxCount;
  }
};

#pragma once

#include "Square.hpp"

template <int BoardSize>
class Box : public Square<BoardSize> {
  public:
  Box() : Square<BoardSize>() {
  }
  Box(int v) : Square<BoardSize>(v) {
  }
  Box(int x, int y) : Square<BoardSize>(x, y) {
  }
  operator int() {
    return Square<BoardSize>::v;
  }
};

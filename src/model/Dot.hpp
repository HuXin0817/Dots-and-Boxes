#pragma once

#include "Square.hpp"

template <int BoardSize>
class Dot : public Square<BoardSize + 1> {
  public:
  Dot() : Square<BoardSize + 1>() {
  }
  Dot(int v) : Square<BoardSize + 1>(v) {
  }
  Dot(int x, int y) : Square<BoardSize + 1>(x, y) {
  }
  operator int() {
    return Square<BoardSize + 1>::v;
  }
};

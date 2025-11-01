#pragma once

#include "Edge.h"

class Step {
  public:
  bool
  Gaming() const {
    return v < Edge::Max;
  }

  int
  RemainStep() const {
    return Edge::Max - v;
  }

  int
  NowStep() const {
    return v;
  }

  void
  Go() {
    v++;
  }

  private:
  int v = 0;
};

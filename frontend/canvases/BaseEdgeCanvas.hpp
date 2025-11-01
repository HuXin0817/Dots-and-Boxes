#pragma once

#include "BaseCanvas.hpp"

class BaseEdgeCanvas : public BaseCanvas {
  public:
  static constexpr int A = R * 2;
  static constexpr int B = A * 5;

  int
  RotateWidth() const {
    return Rotate ? A: B;
  }

  int
  RotateHeight() const {
    return Rotate ? B: A;
  }

  explicit BaseEdgeCanvas(bool rotate, QWidget* parent) : Rotate(rotate), BaseCanvas(parent) {
    resize(QSize(RotateWidth(), RotateHeight()));
  }

  private:
  bool Rotate = false;
};

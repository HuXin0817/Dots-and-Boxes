#pragma once

#include "BaseCanvas.hpp"

class BaseEdgeCanvas : public BaseCanvas {
  public:
  static constexpr int Width = UnitSize * 2;
  static constexpr int Height = Width * 5;

  int
  RotateWidth() const {
    return Rotate ? Width : Height;
  }

  int
  RotateHeight() const {
    return Rotate ? Height : Width;
  }

  explicit BaseEdgeCanvas(bool rotate, QWidget* parent) : BaseCanvas(parent), Rotate(rotate) {
    resize(QSize(RotateWidth(), RotateHeight()));
  }

  private:
  bool Rotate = false;
};

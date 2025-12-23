#pragma once

#include "BaseCanvas.hpp"

template <int BoardSize>
class BaseEdgeCanvas : public BaseCanvas<BoardSize> {
  public:
  static constexpr int Width = BaseCanvas<BoardSize>::UnitSize * 2;
  static constexpr int Height = Width * 5;

  int
  RotateWidth() const {
    return Rotate ? Width : Height;
  }

  int
  RotateHeight() const {
    return Rotate ? Height : Width;
  }

  explicit BaseEdgeCanvas(bool rotate, QWidget* parent)
      : BaseCanvas<BoardSize>(parent), Rotate(rotate) {
    BaseCanvas<BoardSize>::resize(QSize(RotateWidth(), RotateHeight()));
  }

  private:
  bool Rotate = false;
};

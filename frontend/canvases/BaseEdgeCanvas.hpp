#pragma once

#include "BaseCanvas.hpp"

namespace dab::frontend::canvas {

template <int BoardSize>
class BaseEdgeCanvas : public BaseCanvas<BoardSize> {
  public:
  static constexpr int Width = BaseCanvas<BoardSize>::UnitSize * 2;
  static constexpr int Height = Width * 5;

  explicit BaseEdgeCanvas(const bool rotate, QWidget* parent) : BaseCanvas<BoardSize>(parent), Rotate(rotate) {
    BaseCanvas<BoardSize>::resize(QSize(RotateWidth(), RotateHeight()));
  }

  int
  RotateWidth() const {
    return Rotate ? Width : Height;
  }

  int
  RotateHeight() const {
    return Rotate ? Height : Width;
  }

  private:
  const bool Rotate = false;
};

}  // namespace dab::frontend::canvas

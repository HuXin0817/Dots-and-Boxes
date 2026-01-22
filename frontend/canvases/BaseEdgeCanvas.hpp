#pragma once

#include "BaseCanvas.hpp"

namespace dab::frontend::canvas {

template <int BoardSize, typename SizeType>
class BaseEdgeCanvas : public BaseCanvas<BoardSize, SizeType> {
  public:
  static constexpr int Width = BaseCanvas<BoardSize, SizeType>::UnitSize * 2;
  static constexpr int Height = Width * 5;

  explicit BaseEdgeCanvas(const bool rotate, QWidget* parent)
      : BaseCanvas<BoardSize, SizeType>(parent), Rotate(rotate) {
    BaseCanvas<BoardSize, SizeType>::resize(QSize(RotateWidth(), RotateHeight()));
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

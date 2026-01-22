#pragma once

#include "BaseCanvas.hpp"

namespace dab::frontend::canvas {

template <int BoardSize, typename SizeType>
class BaseEdgeCanvas : public BaseCanvas<BoardSize, SizeType> {
  using Base = BaseCanvas<BoardSize, SizeType>;

  public:
  static constexpr int Width = Base::UnitSize * 2;
  static constexpr int Height = Width * 5;

  explicit BaseEdgeCanvas(const bool rotate, QWidget* parent) : Base(parent), Rotate(rotate) {
    Base::resize(QSize(RotateWidth(), RotateHeight()));
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

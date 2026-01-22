#pragma once

#include "../../src/model/Edge.hpp"
#include "../canvases/EdgeCanvas.hpp"
#include "EdgeLayer.hpp"

template <int BoardSize, typename SizeType>
class EdgeCanvasLayer final : public EdgeLayer<BoardSize, SizeType, EdgeCanvas<BoardSize, SizeType>> {
  using Base = EdgeLayer<BoardSize, SizeType, EdgeCanvas<BoardSize, SizeType>>;

  public:
  explicit EdgeCanvasLayer(QWidget* parent) : Base(parent) {
    Base::resize(Base::WindowSize, Base::WindowSize);

    for (const Edge<BoardSize, SizeType> edge : ValueIterator<Edge<BoardSize, SizeType>, SizeType>()) {
      Base::At(edge) = std::make_unique<EdgeCanvas<BoardSize, SizeType>>(edge.Rotate(), this);
    }
  }
};

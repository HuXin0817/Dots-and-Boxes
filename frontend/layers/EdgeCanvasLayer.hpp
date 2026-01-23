#pragma once

#include "../../src/model/Edge.hpp"
#include "../canvases/EdgeCanvas.hpp"
#include "EdgeLayer.hpp"

template <int64_t BoardSize>
class EdgeCanvasLayer final : public EdgeLayer<BoardSize, EdgeCanvas<BoardSize>> {
  using Base = EdgeLayer<BoardSize, EdgeCanvas<BoardSize>>;

  public:
  explicit EdgeCanvasLayer(QWidget* parent) : Base(parent) {
    Base::resize(Base::WindowSize, Base::WindowSize);

    for (const Edge<BoardSize> edge : ValueIterator<Edge<BoardSize>, SizeType<BoardSize>>()) {
      Base::At(edge) = std::make_unique<EdgeCanvas<BoardSize>>(edge.Rotate(), this);
    }
  }
};

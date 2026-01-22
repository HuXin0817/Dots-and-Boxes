#pragma once

#include "../../src/model/Edge.hpp"
#include "../canvases/EdgeCanvas.hpp"
#include "EdgeLayer.hpp"

namespace dab::frontend::layer {

template <int BoardSize, typename SizeType>
class EdgeCanvasLayer final : public EdgeLayer<BoardSize, SizeType, canvas::EdgeCanvas<BoardSize, SizeType>> {
  using Base = EdgeLayer<BoardSize, SizeType, canvas::EdgeCanvas<BoardSize, SizeType>>;

  public:
  explicit EdgeCanvasLayer(QWidget* parent) : Base(parent) {
    Base::resize(Base::WindowSize, Base::WindowSize);

    for (const model::Edge<BoardSize, SizeType> edge :
         model::ValueIterator<model::Edge<BoardSize, SizeType>, SizeType>()) {
      Base::At(edge).New(edge.Rotate(), this);
    }
  }
};

}  // namespace dab::frontend::layer

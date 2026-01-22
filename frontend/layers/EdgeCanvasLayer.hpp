#pragma once

#include "../../src/model/Edge.hpp"
#include "../canvases/EdgeCanvas.hpp"
#include "EdgeLayer.hpp"

namespace dab::frontend::layer {

template <int BoardSize, typename SizeType>
class EdgeCanvasLayer final : public EdgeLayer<BoardSize, SizeType, canvas::EdgeCanvas<BoardSize, SizeType>> {
  public:
  explicit EdgeCanvasLayer(QWidget* parent)
      : EdgeLayer<BoardSize, SizeType, canvas::EdgeCanvas<BoardSize, SizeType>>(parent) {
    EdgeLayer<BoardSize, SizeType, canvas::EdgeCanvas<BoardSize, SizeType>>::resize(
        EdgeLayer<BoardSize, SizeType, canvas::EdgeCanvas<BoardSize, SizeType>>::WindowSize,
        EdgeLayer<BoardSize, SizeType, canvas::EdgeCanvas<BoardSize, SizeType>>::WindowSize);

    for (const model::Edge<BoardSize, SizeType> edge :
         model::ValueIterator<model::Edge<BoardSize, SizeType>, SizeType>()) {
      EdgeLayer<BoardSize, SizeType, canvas::EdgeCanvas<BoardSize, SizeType>>::At(edge).New(edge.Rotate(), this);
    }
  }
};

}  // namespace dab::frontend::layer

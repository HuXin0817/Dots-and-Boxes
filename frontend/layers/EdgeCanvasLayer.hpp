#pragma once

#include "../../src/model/Edge.hpp"
#include "../canvases/EdgeCanvas.hpp"
#include "EdgeLayer.hpp"

namespace dab::frontend::layer {

template <int BoardSize>
class EdgeCanvasLayer final : public EdgeLayer<BoardSize, canvas::EdgeCanvas<BoardSize>> {
  public:
  explicit EdgeCanvasLayer(QWidget* parent) : EdgeLayer<BoardSize, canvas::EdgeCanvas<BoardSize>>(parent) {
    EdgeLayer<BoardSize, canvas::EdgeCanvas<BoardSize>>::resize(
        EdgeLayer<BoardSize, canvas::EdgeCanvas<BoardSize>>::WindowSize,
        EdgeLayer<BoardSize, canvas::EdgeCanvas<BoardSize>>::WindowSize);

    for (const model::Edge<BoardSize> edge : model::ValueIterator<model::Edge<BoardSize>>()) {
      EdgeLayer<BoardSize, canvas::EdgeCanvas<BoardSize>>::At(edge).New(edge.Rotate(), this);
    }
  }
};

}  // namespace dab::frontend::layer

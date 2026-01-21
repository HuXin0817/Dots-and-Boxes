#pragma once

#include "../../src/model/Edge.hpp"
#include "../canvases/EdgeButton.hpp"
#include "EdgeLayer.hpp"

namespace dab::frontend::layer {

template <int BoardSize>
class EdgeButtonLayer final : public EdgeLayer<BoardSize, canvas::EdgeButtonCanvas<BoardSize>> {
  public:
  explicit EdgeButtonLayer(const std::function<std::function<void()>(const model::Edge<BoardSize>)>& callBackFactory,
                           QWidget* parent)
      : EdgeLayer<BoardSize, canvas::EdgeButtonCanvas<BoardSize>>(parent) {
    EdgeLayer<BoardSize, canvas::EdgeButtonCanvas<BoardSize>>::resize(
        EdgeLayer<BoardSize, canvas::EdgeButtonCanvas<BoardSize>>::WindowSize,
        EdgeLayer<BoardSize, canvas::EdgeButtonCanvas<BoardSize>>::WindowSize);

    for (const model::Edge<BoardSize> edge : model::ValueIterator<model::Edge<BoardSize>>()) {
      EdgeLayer<BoardSize, canvas::EdgeButtonCanvas<BoardSize>>::At(edge).New(
          edge.Rotate(), callBackFactory(edge), this);
    }
  }
};

}  // namespace dab::frontend::layer

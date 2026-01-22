#pragma once

#include "../../src/model/Edge.hpp"
#include "../canvases/EdgeButton.hpp"
#include "EdgeLayer.hpp"

namespace dab::frontend::layer {

template <int BoardSize, typename SizeType>
class EdgeButtonLayer final : public EdgeLayer<BoardSize, SizeType, canvas::EdgeButtonCanvas<BoardSize, SizeType>> {
  public:
  explicit EdgeButtonLayer(
      const std::function<std::function<void()>(const model::Edge<BoardSize, SizeType>)>& callBackFactory,
      QWidget* parent)
      : EdgeLayer<BoardSize, SizeType, canvas::EdgeButtonCanvas<BoardSize, SizeType>>(parent) {
    EdgeLayer<BoardSize, SizeType, canvas::EdgeButtonCanvas<BoardSize, SizeType>>::resize(
        EdgeLayer<BoardSize, SizeType, canvas::EdgeButtonCanvas<BoardSize, SizeType>>::WindowSize,
        EdgeLayer<BoardSize, SizeType, canvas::EdgeButtonCanvas<BoardSize, SizeType>>::WindowSize);

    for (const model::Edge<BoardSize, SizeType> edge : model::ValueIterator<model::Edge<BoardSize, SizeType>>()) {
      EdgeLayer<BoardSize, SizeType, canvas::EdgeButtonCanvas<BoardSize, SizeType>>::At(edge).New(
          edge.Rotate(), callBackFactory(edge), this);
    }
  }
};

}  // namespace dab::frontend::layer

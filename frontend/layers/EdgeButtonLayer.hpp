#pragma once

#include "../../src/model/Edge.hpp"
#include "../canvases/EdgeButton.hpp"
#include "EdgeLayer.hpp"

namespace dab::frontend::layer {

template <int BoardSize, typename SizeType>
class EdgeButtonLayer final : public EdgeLayer<BoardSize, SizeType, canvas::EdgeButtonCanvas<BoardSize, SizeType>> {
  using Base = EdgeLayer<BoardSize, SizeType, canvas::EdgeButtonCanvas<BoardSize, SizeType>>;

  public:
  explicit EdgeButtonLayer(
      const std::function<std::function<void()>(const model::Edge<BoardSize, SizeType>)>& callBackFactory,
      QWidget* parent)
      : Base(parent) {
    Base::resize(Base::WindowSize, Base::WindowSize);

    for (const model::Edge<BoardSize, SizeType> edge :
         model::ValueIterator<model::Edge<BoardSize, SizeType>, SizeType>()) {
      Base::At(edge).New(edge.Rotate(), callBackFactory(edge), this);
    }
  }
};

}  // namespace dab::frontend::layer

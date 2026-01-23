#pragma once

#include "../../src/model/Edge.hpp"
#include "../canvases/EdgeButton.hpp"
#include "EdgeLayer.hpp"

template <int64_t BoardSize>
class EdgeButtonLayer final : public EdgeLayer<BoardSize, EdgeButtonCanvas<BoardSize>> {
  using Base = EdgeLayer<BoardSize, EdgeButtonCanvas<BoardSize>>;

  public:
  explicit EdgeButtonLayer(const std::function<std::function<void()>(const Edge<BoardSize>)>& callBackFactory,
                           QWidget* parent)
      : Base(parent) {
    Base::resize(Base::WindowSize, Base::WindowSize);

    for (const Edge<BoardSize> edge : ValueIterator<Edge<BoardSize>, SizeType<BoardSize>>()) {
      Base::At(edge) = std::make_unique<EdgeButtonCanvas<BoardSize>>(edge.Rotate(), callBackFactory(edge), this);
    }
  }
};

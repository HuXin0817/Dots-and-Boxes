#pragma once

#include "../../src/model/Edge.hpp"
#include "../canvases/EdgeButton.hpp"
#include "EdgeLayer.hpp"

template <int BoardSize, typename SizeType>
class EdgeButtonLayer final : public EdgeLayer<BoardSize, SizeType, EdgeButtonCanvas<BoardSize, SizeType>> {
  using Base = EdgeLayer<BoardSize, SizeType, EdgeButtonCanvas<BoardSize, SizeType>>;

  public:
  explicit EdgeButtonLayer(const std::function<std::function<void()>(const Edge<BoardSize, SizeType>)>& callBackFactory,
                           QWidget* parent)
      : Base(parent) {
    Base::resize(Base::WindowSize, Base::WindowSize);

    for (const Edge<BoardSize, SizeType> edge : ValueIterator<Edge<BoardSize, SizeType>, SizeType>()) {
      Base::At(edge) =
          std::make_unique<EdgeButtonCanvas<BoardSize, SizeType>>(edge.Rotate(), callBackFactory(edge), this);
    }
  }
};

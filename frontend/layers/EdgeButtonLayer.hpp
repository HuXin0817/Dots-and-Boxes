#pragma once

#include <QWidget>

#include "../../src/common/Array.hpp"
#include "../../src/model/Edge.hpp"
#include "../canvases/EdgeButton.hpp"
#include "EdgeLayer.hpp"

template <int BoardSize>
class EdgeButtonLayer final : public EdgeLayer<BoardSize, EdgeButtonCanvas<BoardSize>> {
  public:
  explicit EdgeButtonLayer(
      const std::function<std::function<void()>(Edge<BoardSize>)>& callBackFactory, QWidget* parent)
      : EdgeLayer<BoardSize, EdgeButtonCanvas<BoardSize>>(parent) {
    EdgeLayer<BoardSize, EdgeButtonCanvas<BoardSize>>::resize(
        EdgeLayer<BoardSize, EdgeButtonCanvas<BoardSize>>::WindowSize,
        EdgeLayer<BoardSize, EdgeButtonCanvas<BoardSize>>::WindowSize);

    for (Edge<BoardSize> edge = 0; edge.Int() < Edge<BoardSize>::Max; ++edge) {
      EdgeLayer<BoardSize, EdgeButtonCanvas<BoardSize>>::GetCanvases()
          .At(edge.Int())
          .New(edge.Rotate(), callBackFactory(edge), this);
    }
  }
};

#pragma once

#include <QWidget>

#include "../../src/common/Array.hpp"
#include "../../src/model/Edge.hpp"
#include "../canvases/EdgeButton.hpp"
#include "EdgeLayer.hpp"

template <int BoardSize>
class EdgeButtonLayer final : public EdgeLayer<BoardSize, EdgeButtonCanvas> {
  public:
  explicit EdgeButtonLayer(
      const std::function<std::function<void()>(Edge<BoardSize>)>& CallBackFactory, QWidget* parent)
      : EdgeLayer<BoardSize, EdgeButtonCanvas>(parent) {
    EdgeLayer<BoardSize, EdgeButtonCanvas>::resize(
        EdgeLayer<BoardSize, EdgeButtonCanvas>::WindowSize,
        EdgeLayer<BoardSize, EdgeButtonCanvas>::WindowSize);

    for (Edge<BoardSize> edge = 0; edge < Edge<BoardSize>::Max; edge++) {
      EdgeLayer<BoardSize, EdgeButtonCanvas>::Canvases.At(edge).New(
          edge.Rotate(), CallBackFactory(edge), this);
    }
  }
};

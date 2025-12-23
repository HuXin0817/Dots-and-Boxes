#pragma once

#include <QWidget>

#include "../../src/common/Array.hpp"
#include "../../src/model/Edge.hpp"
#include "../canvases/EdgeCanvas.hpp"
#include "EdgeLayer.hpp"

template <int BoardSize>
class EdgeCanvasLayer final : public EdgeLayer<BoardSize, EdgeCanvas<BoardSize>> {
  public:
  explicit EdgeCanvasLayer(QWidget* parent) : EdgeLayer<BoardSize, EdgeCanvas<BoardSize>>(parent) {
    EdgeLayer<BoardSize, EdgeCanvas<BoardSize>>::resize(
        EdgeLayer<BoardSize, EdgeCanvas<BoardSize>>::WindowSize,
        EdgeLayer<BoardSize, EdgeCanvas<BoardSize>>::WindowSize);

    for (Edge<BoardSize> edge = 0; edge.Int() < Edge<BoardSize>::Max; ++edge) {
      EdgeLayer<BoardSize, EdgeCanvas<BoardSize>>::GetCanvases()
          .At(edge.Int())
          .New(edge.Rotate(), this);
    }
  }
};

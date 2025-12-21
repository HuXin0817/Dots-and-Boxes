#pragma once

#include <QWidget>

#include "../../src/common/Array.hpp"
#include "../../src/model/Edge.hpp"
#include "../canvases/EdgeCanvas.hpp"
#include "EdgeLayer.hpp"

template <int BoardSize>
class EdgeCanvasLayer final : public EdgeLayer<BoardSize, EdgeCanvas> {
  public:
  explicit EdgeCanvasLayer(QWidget* parent) : EdgeLayer<BoardSize, EdgeCanvas>(parent) {
    EdgeLayer<BoardSize, EdgeCanvas>::resize(EdgeLayer<BoardSize, EdgeCanvas>::WindowSize,
                                             EdgeLayer<BoardSize, EdgeCanvas>::WindowSize);

    for (Edge<BoardSize> edge = 0; edge < Edge<BoardSize>::Max; edge++) {
      EdgeLayer<BoardSize, EdgeCanvas>::GetCanvases().At(edge).New(edge.Rotate(), this);
    }
  }
};

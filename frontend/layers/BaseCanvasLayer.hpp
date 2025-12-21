#pragma once

#include "../../src/model/Box.hpp"
#include "../canvases/BoxCanvas.hpp"
#include "../canvases/EdgeCanvas.hpp"

template <int BoardSize>
class BaseCanvasLayer : public BaseCanvas {
  public:
  static constexpr int BoardWidth = Box<BoardSize>::Size * EdgeCanvas::Height;
  static constexpr int WindowSize = BoardWidth + 2 * BoxCanvas::Width;

  explicit BaseCanvasLayer(QWidget* parent) : BaseCanvas(parent) {
  }
};

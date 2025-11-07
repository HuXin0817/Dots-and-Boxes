#pragma once

#include "../canvases/BoxCanvas.hpp"
#include "../canvases/EdgeCanvas.hpp"

class BaseCanvasLayer : public BaseCanvas {
  public:
  static constexpr int BoardWidth = Box::Size * EdgeCanvas::Height;
  static constexpr int WindowSize = BoardWidth + 2 * BoxCanvas::Width;

  explicit BaseCanvasLayer(QWidget* parent) : BaseCanvas(parent) {
  }
};

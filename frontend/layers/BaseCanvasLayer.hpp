#pragma once

#include "../canvases/BoxCanvas.hpp"
#include "../canvases/EdgeCanvas.hpp"

class BaseCanvasLayer : public BaseCanvas {
  public:
  static constexpr int BoardWidth = Box::Size * EdgeCanvas::Height;
  static constexpr int WindowSize =
      (Box::Size - 1) * EdgeCanvas::Height + 2 * BoxCanvas::Width + 80;

  explicit BaseCanvasLayer(QWidget* parent) : BaseCanvas(parent) {
  }
};

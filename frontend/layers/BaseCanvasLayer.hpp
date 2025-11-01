#pragma once

#include "../canvases/BoxCanvas.hpp"
#include "../canvases/EdgeCanvas.hpp"

class BaseCanvasLayer : public BaseCanvas {
  public:
  static constexpr int BoardWindowSize = Box::Size * EdgeCanvas::B;
  static constexpr int WindowSize = (Box::Size - 1) * EdgeCanvas::B + 2 * BoxCanvas::A + 80;

  explicit BaseCanvasLayer(QWidget* parent) : BaseCanvas(parent) {
  }
};

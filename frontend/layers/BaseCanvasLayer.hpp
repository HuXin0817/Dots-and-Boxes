#pragma once

#include "../../src/model/Square.hpp"
#include "../canvases/BoxCanvas.hpp"
#include "../canvases/EdgeCanvas.hpp"

template <int BoardSize>
class BaseCanvasLayer : public BaseCanvas<BoardSize> {
  public:
  static constexpr int BoardWidth = Box<BoardSize>::Size * EdgeCanvas<BoardSize>::Height;
  static constexpr int WindowSize = BoardWidth + 2 * BoxCanvas<BoardSize>::Width;

  explicit BaseCanvasLayer(QWidget* parent) : BaseCanvas<BoardSize>(parent) {
  }
};

#pragma once

#include "../../src/model/Square.hpp"
#include "../canvases/BoxCanvas.hpp"
#include "../canvases/EdgeCanvas.hpp"

namespace dab::frontend::layer {

template <int BoardSize>
class BaseCanvasLayer : public canvas::BaseCanvas<BoardSize> {
  public:
  static constexpr int BoardWidth = model::Box<BoardSize>::Size * canvas::EdgeCanvas<BoardSize>::Height;
  static constexpr int WindowSize = BoardWidth + 2 * canvas::BoxCanvas<BoardSize>::Width;

  explicit BaseCanvasLayer(QWidget* parent) : canvas::BaseCanvas<BoardSize>(parent) {
  }
};

}  // namespace dab::frontend::layer

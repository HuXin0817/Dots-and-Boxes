#pragma once

#include "../../src/model/Square.hpp"
#include "../canvases/BoxCanvas.hpp"
#include "../canvases/EdgeCanvas.hpp"

namespace dab::frontend::layer {

template <int BoardSize, typename SizeType>
class BaseCanvasLayer : public canvas::BaseCanvas<BoardSize, SizeType> {
  public:
  static constexpr int BoardWidth =
      model::Box<BoardSize, SizeType>::Size * canvas::EdgeCanvas<BoardSize, SizeType>::Height;
  static constexpr int WindowSize = BoardWidth + 2 * canvas::BoxCanvas<BoardSize, SizeType>::Width;

  explicit BaseCanvasLayer(QWidget* parent) : canvas::BaseCanvas<BoardSize, SizeType>(parent) {
  }
};

}  // namespace dab::frontend::layer

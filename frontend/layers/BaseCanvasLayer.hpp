#pragma once

#include "../../src/model/Square.hpp"
#include "../canvases/BoxCanvas.hpp"
#include "../canvases/EdgeCanvas.hpp"

template <int BoardSize, typename SizeType>
class BaseCanvasLayer : public BaseCanvas<BoardSize, SizeType> {
  using Base = BaseCanvas<BoardSize, SizeType>;

  public:
  static constexpr int BoardWidth = Box<BoardSize, SizeType>::Size * EdgeCanvas<BoardSize, SizeType>::Height;
  static constexpr int WindowSize = BoardWidth + 2 * BoxCanvas<BoardSize, SizeType>::Width;

  explicit BaseCanvasLayer(QWidget* parent) : Base(parent) {
  }
};

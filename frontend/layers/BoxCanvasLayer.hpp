#pragma once

#include "../../src/common/Array.hpp"
#include "../../src/common/Ptr.hpp"
#include "../../src/model/Square.hpp"
#include "../../src/model/ValueIterator.hpp"
#include "../canvases/BoxCanvas.hpp"
#include "../canvases/EdgeCanvas.hpp"
#include "BaseCanvasLayer.hpp"

namespace dab::frontend::layer {

template <int BoardSize, typename SizeType>
class BoxCanvasLayer final : public BaseCanvasLayer<BoardSize, SizeType> {
  public:
  explicit BoxCanvasLayer(QWidget* parent) : BaseCanvasLayer<BoardSize, SizeType>(parent) {
    BaseCanvasLayer<BoardSize, SizeType>::resize(BaseCanvasLayer<BoardSize, SizeType>::WindowSize,
                                                 BaseCanvasLayer<BoardSize, SizeType>::WindowSize);
    for (const model::Box<BoardSize, SizeType> box : model::ValueIterator<model::Box<BoardSize, SizeType>>()) {
      BoxCanvases.At(box.Value()).New(this);
    }
  }

  common::Ptr<canvas::BoxCanvas<BoardSize, SizeType>>&
  At(const model::Box<BoardSize, SizeType> box) {
    return BoxCanvases.At(box.Value());
  }

  protected:
  void
  resizeEvent(QResizeEvent* event) override {
    BaseCanvasLayer<BoardSize, SizeType>::resizeEvent(event);

    const int x0 =
        (BaseCanvasLayer<BoardSize, SizeType>::width() - BaseCanvasLayer<BoardSize, SizeType>::BoardWidth) / 2 +
        BaseCanvasLayer<BoardSize, SizeType>::UnitSize;
    const int y0 =
        (BaseCanvasLayer<BoardSize, SizeType>::height() - BaseCanvasLayer<BoardSize, SizeType>::BoardWidth) / 2 +
        BaseCanvasLayer<BoardSize, SizeType>::UnitSize;

    for (int i = 0; i < model::Box<BoardSize, SizeType>::Size; i++) {
      for (int j = 0; j < model::Box<BoardSize, SizeType>::Size; j++) {
        int x = x0 + i * canvas::EdgeCanvas<BoardSize, SizeType>::Height;
        int y = y0 + j * canvas::EdgeCanvas<BoardSize, SizeType>::Height;
        BoxCanvases.At(model::Box<BoardSize, SizeType>(i, j).Value())->move(x, y);
      }
    }
  }

  private:
  common::Array<common::Ptr<canvas::BoxCanvas<BoardSize, SizeType>>, model::Box<BoardSize, SizeType>::Max, int>
      BoxCanvases;
};

}  // namespace dab::frontend::layer

#pragma once

#include "../../src/common/Array.hpp"
#include "../../src/common/Ptr.hpp"
#include "../../src/model/Square.hpp"
#include "../canvases/BoxCanvas.hpp"
#include "../canvases/EdgeCanvas.hpp"
#include "BaseCanvasLayer.hpp"

namespace dab::frontend::layer {

template <int BoardSize>
class BoxCanvasLayer final : public BaseCanvasLayer<BoardSize> {
  public:
  explicit BoxCanvasLayer(QWidget* parent) : BaseCanvasLayer<BoardSize>(parent) {
    BaseCanvasLayer<BoardSize>::resize(BaseCanvasLayer<BoardSize>::WindowSize, BaseCanvasLayer<BoardSize>::WindowSize);
    for (model::Box<BoardSize> box = 0; box.Value() < model::Box<BoardSize>::Max; ++box) {
      BoxCanvases.At(box.Value()).New(this);
    }
  }

  common::Ptr<canvas::BoxCanvas<BoardSize>>&
  At(const model::Box<BoardSize> box) {
    return BoxCanvases.At(box.Value());
  }

  protected:
  void
  resizeEvent(QResizeEvent* event) override {
    BaseCanvasLayer<BoardSize>::resizeEvent(event);

    const int x0 = (BaseCanvasLayer<BoardSize>::width() - BaseCanvasLayer<BoardSize>::BoardWidth) / 2 +
                   BaseCanvasLayer<BoardSize>::UnitSize;
    const int y0 = (BaseCanvasLayer<BoardSize>::height() - BaseCanvasLayer<BoardSize>::BoardWidth) / 2 +
                   BaseCanvasLayer<BoardSize>::UnitSize;

    for (int i = 0; i < model::Box<BoardSize>::Size; i++) {
      for (int j = 0; j < model::Box<BoardSize>::Size; j++) {
        int x = x0 + i * canvas::EdgeCanvas<BoardSize>::Height;
        int y = y0 + j * canvas::EdgeCanvas<BoardSize>::Height;
        BoxCanvases.At(model::Box<BoardSize>(i, j).Value())->move(x, y);
      }
    }
  }

  private:
  common::Array<common::Ptr<canvas::BoxCanvas<BoardSize>>, model::Box<BoardSize>::Max> BoxCanvases;
};

}  // namespace dab::frontend::layer

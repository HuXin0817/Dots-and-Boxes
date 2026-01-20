#pragma once

#include <QWidget>

#include "../../src/common/Array.hpp"
#include "../../src/common/Ptr.hpp"
#include "../../src/model/Square.hpp"
#include "../canvases/DotCanvas.hpp"
#include "../canvases/EdgeCanvas.hpp"
#include "BaseCanvasLayer.hpp"

namespace dab::frontend::layer {

template <int BoardSize>
class DotCanvasLayer final : public BaseCanvasLayer<BoardSize> {
  public:
  explicit DotCanvasLayer(QWidget* parent) : BaseCanvasLayer<BoardSize>(parent) {
    BaseCanvasLayer<BoardSize>::resize(BaseCanvasLayer<BoardSize>::WindowSize, BaseCanvasLayer<BoardSize>::WindowSize);
    for (model::Dot<BoardSize> dot = 0; dot.Value() < model::Dot<BoardSize>::Max; ++dot) {
      DotCanvases.At(dot.Value()).New(this);
    }
  }

  common::Ptr<canvas::DotCanvas<BoardSize>>&
  At(const model::Dot<BoardSize> dot) {
    return DotCanvases.At(dot.Value());
  }

  protected:
  void
  resizeEvent(QResizeEvent* event) override {
    BaseCanvasLayer<BoardSize>::resizeEvent(event);

    const int x0 = (BaseCanvasLayer<BoardSize>::width() - BaseCanvasLayer<BoardSize>::BoardWidth) / 2 -
                   BaseCanvasLayer<BoardSize>::UnitSize;
    const int y0 = (BaseCanvasLayer<BoardSize>::height() - BaseCanvasLayer<BoardSize>::BoardWidth) / 2 -
                   BaseCanvasLayer<BoardSize>::UnitSize;

    for (int i = 0; i < model::Dot<BoardSize>::Size; i++) {
      for (int j = 0; j < model::Dot<BoardSize>::Size; j++) {
        const int x = x0 + i * canvas::EdgeCanvas<BoardSize>::Height;
        const int y = y0 + j * canvas::EdgeCanvas<BoardSize>::Height;
        DotCanvases.At(model::Dot<BoardSize>(i, j).Value())->move(x, y);
      }
    }
  }

  private:
  common::Array<common::Ptr<canvas::DotCanvas<BoardSize>>, model::Dot<BoardSize>::Max> DotCanvases;
};

}  // namespace dab::frontend::layer

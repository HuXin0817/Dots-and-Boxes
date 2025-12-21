#pragma once

#include <QWidget>

#include "../../src/common/Array.hpp"
#include "../../src/common/Ptr.hpp"
#include "../../src/model/Dot.hpp"
#include "../canvases/DotCanvas.hpp"
#include "../canvases/EdgeCanvas.hpp"
#include "BaseCanvasLayer.hpp"

template <int BoardSize>
class DotCanvasLayer final : public BaseCanvasLayer<BoardSize> {
  public:
  explicit DotCanvasLayer(QWidget* parent) : BaseCanvasLayer<BoardSize>(parent) {
    BaseCanvasLayer<BoardSize>::resize(BaseCanvasLayer<BoardSize>::WindowSize,
                                       BaseCanvasLayer<BoardSize>::WindowSize);
    for (Dot<BoardSize> dot = 0; dot < Dot<BoardSize>::Max; dot++) {
      DotCanvases.At(dot).New(this);
    }
  }

  const Array<Ptr<DotCanvas>, Dot<BoardSize>::Max>&
  GetDotCanvases() const {
    return DotCanvases;
  }

  Array<Ptr<DotCanvas>, Dot<BoardSize>::Max>&
  GetDotCanvases() {
    return DotCanvases;
  }

  protected:
  void
  resizeEvent(QResizeEvent* event) override {
    BaseCanvasLayer<BoardSize>::resizeEvent(event);

    int x0 = (BaseCanvasLayer<BoardSize>::width() - BaseCanvasLayer<BoardSize>::BoardWidth) / 2 -
             BaseCanvasLayer<BoardSize>::UnitSize;
    int y0 = (BaseCanvasLayer<BoardSize>::height() - BaseCanvasLayer<BoardSize>::BoardWidth) / 2 -
             BaseCanvasLayer<BoardSize>::UnitSize;

    for (int i = 0; i < Dot<BoardSize>::Size; i++) {
      for (int j = 0; j < Dot<BoardSize>::Size; j++) {
        int x = x0 + i * EdgeCanvas::Height;
        int y = y0 + j * EdgeCanvas::Height;
        DotCanvases.At(Dot<BoardSize>(i, j))->move(x, y);
      }
    }
  }

  private:
  Array<Ptr<DotCanvas>, Dot<BoardSize>::Max> DotCanvases;
};

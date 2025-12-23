#pragma once

#include "../../src/common/Array.hpp"
#include "../../src/common/Ptr.hpp"
#include "../../src/model/Square.hpp"
#include "../canvases/BoxCanvas.hpp"
#include "../canvases/EdgeCanvas.hpp"
#include "BaseCanvasLayer.hpp"

template <int BoardSize>
class BoxCanvasLayer final : public BaseCanvasLayer<BoardSize> {
  public:
  explicit BoxCanvasLayer(QWidget* parent) : BaseCanvasLayer<BoardSize>(parent) {
    BaseCanvasLayer<BoardSize>::resize(BaseCanvasLayer<BoardSize>::WindowSize,
                                       BaseCanvasLayer<BoardSize>::WindowSize);
    for (Box<BoardSize> box = 0; box.Int() < Box<BoardSize>::Max; ++box) {
      BoxCanvases.At(box.Int()).New(this);
    }
  }

  const Array<Ptr<BoxCanvas<BoardSize>>, Box<BoardSize>::Max>&
  GetBoxCanvases() const {
    return BoxCanvases;
  }

  Array<Ptr<BoxCanvas<BoardSize>>, Box<BoardSize>::Max>&
  GetBoxCanvases() {
    return BoxCanvases;
  }

  protected:
  void
  resizeEvent(QResizeEvent* event) override {
    BaseCanvasLayer<BoardSize>::resizeEvent(event);

    int x0 = (BaseCanvasLayer<BoardSize>::width() - BaseCanvasLayer<BoardSize>::BoardWidth) / 2 +
             BaseCanvasLayer<BoardSize>::UnitSize;
    int y0 = (BaseCanvasLayer<BoardSize>::height() - BaseCanvasLayer<BoardSize>::BoardWidth) / 2 +
             BaseCanvasLayer<BoardSize>::UnitSize;

    for (int i = 0; i < Box<BoardSize>::Size; i++) {
      for (int j = 0; j < Box<BoardSize>::Size; j++) {
        int x = x0 + i * EdgeCanvas<BoardSize>::Height;
        int y = y0 + j * EdgeCanvas<BoardSize>::Height;
        BoxCanvases.At(Box<BoardSize>(i, j).Int())->move(x, y);
      }
    }
  }

  private:
  Array<Ptr<BoxCanvas<BoardSize>>, Box<BoardSize>::Max> BoxCanvases;
};

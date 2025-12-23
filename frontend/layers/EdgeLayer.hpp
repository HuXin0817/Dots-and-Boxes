#pragma once

#include "../../src/common/Array.hpp"
#include "../../src/common/Ptr.hpp"
#include "../../src/model/Edge.hpp"
#include "BaseCanvasLayer.hpp"

template <int BoardSize, class Canvas>
class EdgeLayer : public BaseCanvasLayer<BoardSize> {
  public:
  explicit EdgeLayer(QWidget* parent) : BaseCanvasLayer<BoardSize>(parent) {
  }

  const Array<Ptr<Canvas>, Edge<BoardSize>::Max>&
  GetCanvases() const {
    return Canvases;
  }

  Array<Ptr<Canvas>, Edge<BoardSize>::Max>&
  GetCanvases() {
    return Canvases;
  }

  protected:
  void
  resizeEvent(QResizeEvent* event) override {
    BaseCanvasLayer<BoardSize>::resizeEvent(event);

    int x0 = (BaseCanvasLayer<BoardSize>::width() - BaseCanvasLayer<BoardSize>::BoardWidth) / 2 -
             BaseCanvasLayer<BoardSize>::UnitSize;
    int y0 = (BaseCanvasLayer<BoardSize>::height() - BaseCanvasLayer<BoardSize>::BoardWidth) / 2 -
             BaseCanvasLayer<BoardSize>::UnitSize;

    for (Edge<BoardSize> edge = 0; edge.Int() < Edge<BoardSize>::Max; ++edge) {
      int x = x0 + edge.Dot1().X() * EdgeCanvas::Height;
      int y = y0 + edge.Dot1().Y() * EdgeCanvas::Height;
      if (edge.Dot1().X() == edge.Dot2().X()) {
        y += BaseCanvasLayer<BoardSize>::UnitSize;
      } else {
        x += BaseCanvasLayer<BoardSize>::UnitSize;
      }
      Canvases.At(edge.Int())->move(x, y);
    }
  }

  private:
  Array<Ptr<Canvas>, Edge<BoardSize>::Max> Canvases;
};

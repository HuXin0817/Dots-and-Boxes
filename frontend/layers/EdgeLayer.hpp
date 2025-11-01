#pragma once

#include "../../src/common/Array.hpp"
#include "../../src/model/Edge.hpp"
#include "BaseCanvasLayer.hpp"

template <class Canvas>
class EdgeLayer : public BaseCanvasLayer {
  friend class MainWindow;

  public:
  explicit EdgeLayer(QWidget* parent) : BaseCanvasLayer(parent) {
  }

  protected:
  void
  resizeEvent(QResizeEvent* event) override {
    BaseCanvasLayer::resizeEvent(event);

    int x0 = (width() - BoardWidth) / 2 - UnitSize;
    int y0 = (height() - BoardWidth) / 2 - UnitSize;

    for (Edge edge = 0; edge < Edge::Max; edge++) {
      int x = x0 + edge.Dot1().X() * EdgeCanvas::Height;
      int y = y0 + edge.Dot1().Y() * EdgeCanvas::Height;
      if (edge.Dot1().X() == edge.Dot2().X()) {
        y += UnitSize;
      } else {
        x += UnitSize;
      }
      Canvases.At(edge)->move(x, y);
    }
  }

  Array<Ptr<Canvas>, Edge::Max> Canvases;
};

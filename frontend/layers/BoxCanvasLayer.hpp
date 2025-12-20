#pragma once

#include "../../src/common/Array.hpp"
#include "../../src/common/Ptr.hpp"
#include "../../src/model/Square.hpp"
#include "../canvases/BoxCanvas.hpp"
#include "../canvases/EdgeCanvas.hpp"
#include "BaseCanvasLayer.hpp"

class BoxCanvasLayer final : public BaseCanvasLayer {
  friend class MainWindow;

  Q_OBJECT

  public:
  explicit BoxCanvasLayer(QWidget* parent) : BaseCanvasLayer(parent) {
    resize(WindowSize, WindowSize);
    for (Box box = 0; box < Box::Max; box++) {
      BoxCanvases.At(box).New(this);
    }
  }

  protected:
  void
  resizeEvent(QResizeEvent* event) override {
    BaseCanvasLayer::resizeEvent(event);

    int x0 = (width() - BoardWidth) / 2 + UnitSize;
    int y0 = (height() - BoardWidth) / 2 + UnitSize;

    for (int i = 0; i < Box::Size; i++) {
      for (int j = 0; j < Box::Size; j++) {
        int x = x0 + i * EdgeCanvas::Height;
        int y = y0 + j * EdgeCanvas::Height;
        BoxCanvases.At(Box(i, j))->move(x, y);
      }
    }
  }

  private:
  Array<Ptr<BoxCanvas>, Box::Max> BoxCanvases;
};

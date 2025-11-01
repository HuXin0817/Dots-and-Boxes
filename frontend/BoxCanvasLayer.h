#pragma once

#include "../src/common/Array.h"
#include "../src/common/Ptr.h"
#include "../src/model/Square.h"
#include "BaseCanvasLayer.h"
#include "BoxCanvas.h"
#include "DotCanvas.h"
#include "EdgeCanvas.h"

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

    int x0 = (width() - BoardWindowSize) / 2 + R;
    int y0 = (height() - BoardWindowSize) / 2 + R;

    for (int i = 0; i < Box::Size; i++) {
      for (int j = 0; j < Box::Size; j++) {
        int x = x0 + i * EdgeCanvas::B;
        int y = y0 + j * EdgeCanvas::B;
        BoxCanvases.At(Box(i, j))->move(x, y);
      }
    }
  }

  private:
  Array<Ptr<BoxCanvas>, Box::Max> BoxCanvases;
};

#pragma once

#include <QWidget>

#include "../../src/common/Array.hpp"
#include "../../src/model/Square.hpp"
#include "../canvases/DotCanvas.hpp"
#include "../canvases/EdgeCanvas.hpp"
#include "BaseCanvasLayer.hpp"

class DotCanvasLayer final : public BaseCanvasLayer {
  friend class MainWindow;

  Q_OBJECT

  public:
  explicit DotCanvasLayer(QWidget* parent) : BaseCanvasLayer(parent) {
    resize(WindowSize, WindowSize);
    for (Dot dot = 0; dot < Dot::Max; dot++) {
      DotCanvases.At(dot).New(this);
    }
  }

  protected:
  void
  resizeEvent(QResizeEvent* event) override {
    BaseCanvasLayer::resizeEvent(event);

    int x0 = (width() - BoardWindowSize) / 2 - R;
    int y0 = (height() - BoardWindowSize) / 2 - R;

    for (int i = 0; i < Dot::Size; i++) {
      for (int j = 0; j < Dot::Size; j++) {
        int x = x0 + i * EdgeCanvas::B;
        int y = y0 + j * EdgeCanvas::B;
        DotCanvases.At(Dot(i, j))->move(x, y);
      }
    }
  }

  private:
  Array<Ptr<DotCanvas>, Dot::Max> DotCanvases;
};

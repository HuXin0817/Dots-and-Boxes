#pragma once

#include <QWidget>

#include "../../src/common/Array.hpp"
#include "../../src/model/Edge.hpp"
#include "../canvases/EdgeCanvas.hpp"
#include "EdgeLayer.hpp"

class EdgeCanvasLayer final : public EdgeLayer<EdgeCanvas> {
  friend class MainWindow;

  Q_OBJECT

  public:
  explicit EdgeCanvasLayer(QWidget* parent) : EdgeLayer(parent) {
    resize(WindowSize, WindowSize);

    for (Edge edge = 0; edge < Edge::Max; edge++) {
      Canvases.At(edge).New(edge.Rotate(), this);
    }
  }
};

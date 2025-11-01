#pragma once

#include <QWidget>

#include "../src/common/Array.h"
#include "../src/model/Edge.h"
#include "EdgeButton.h"
#include "EdgeLayer.h"

class EdgeButtonLayer final : public EdgeLayer<EdgeButton> {
  friend class MainWindow;

  Q_OBJECT

  public:
  explicit EdgeButtonLayer(const std::function<std::function<void()>(Edge)>& CallBackFactory,
                           QWidget* parent)
      : EdgeLayer(parent) {
    resize(WindowSize, WindowSize);

    for (Edge edge = 0; edge < Edge::Max; edge++) {
      Canvases.At(edge).New(edge.Rotate(), CallBackFactory(edge), this);
    }
  }
};

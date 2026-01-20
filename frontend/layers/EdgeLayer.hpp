#pragma once

#include "../../src/common/Array.hpp"
#include "../../src/common/Ptr.hpp"
#include "../../src/model/Edge.hpp"
#include "BaseCanvasLayer.hpp"

namespace dab::frontend::layer {

template <int BoardSize, class Canvas>
class EdgeLayer : public BaseCanvasLayer<BoardSize> {
  public:
  explicit EdgeLayer(QWidget* parent) : BaseCanvasLayer<BoardSize>(parent) {
  }

  common::Ptr<Canvas>&
  At(const model::Edge<BoardSize> Edge) {
    return Canvases.At(Edge.Value());
  }

  protected:
  void
  resizeEvent(QResizeEvent* event) override {
    BaseCanvasLayer<BoardSize>::resizeEvent(event);

    const int x0 = (BaseCanvasLayer<BoardSize>::width() - BaseCanvasLayer<BoardSize>::BoardWidth) / 2 -
                   BaseCanvasLayer<BoardSize>::UnitSize;
    const int y0 = (BaseCanvasLayer<BoardSize>::height() - BaseCanvasLayer<BoardSize>::BoardWidth) / 2 -
                   BaseCanvasLayer<BoardSize>::UnitSize;

    for (model::Edge<BoardSize> edge = 0; edge.Value() < model::Edge<BoardSize>::Max; ++edge) {
      int x = x0 + edge.Dot1().X() * canvas::EdgeCanvas<BoardSize>::Height;
      int y = y0 + edge.Dot1().Y() * canvas::EdgeCanvas<BoardSize>::Height;
      if (edge.Dot1().X() == edge.Dot2().X()) {
        y += BaseCanvasLayer<BoardSize>::UnitSize;
      } else {
        x += BaseCanvasLayer<BoardSize>::UnitSize;
      }
      Canvases.At(edge.Value())->move(x, y);
    }
  }

  private:
  common::Array<common::Ptr<Canvas>, model::Edge<BoardSize>::Max> Canvases;
};

}  // namespace dab::frontend::layer

#pragma once

#include "../../src/common/Array.hpp"
#include "../../src/common/Ptr.hpp"
#include "../../src/model/Edge.hpp"
#include "../../src/model/ValueIterator.hpp"
#include "BaseCanvasLayer.hpp"

namespace dab::frontend::layer {

template <int BoardSize, typename SizeType, typename Canvas>
class EdgeLayer : public BaseCanvasLayer<BoardSize, SizeType> {
  public:
  explicit EdgeLayer(QWidget* parent) : BaseCanvasLayer<BoardSize, SizeType>(parent) {
  }

  common::Ptr<Canvas>&
  At(const model::Edge<BoardSize, SizeType> Edge) {
    return Canvases.At(Edge.Value());
  }

  protected:
  void
  resizeEvent(QResizeEvent* event) override {
    BaseCanvasLayer<BoardSize, SizeType>::resizeEvent(event);

    const int x0 =
        (BaseCanvasLayer<BoardSize, SizeType>::width() - BaseCanvasLayer<BoardSize, SizeType>::BoardWidth) / 2 -
        BaseCanvasLayer<BoardSize, SizeType>::UnitSize;
    const int y0 =
        (BaseCanvasLayer<BoardSize, SizeType>::height() - BaseCanvasLayer<BoardSize, SizeType>::BoardWidth) / 2 -
        BaseCanvasLayer<BoardSize, SizeType>::UnitSize;

    for (const model::Edge<BoardSize, SizeType> edge : model::ValueIterator<model::Edge<BoardSize, SizeType>>()) {
      int x = x0 + edge.Dot1().X() * canvas::EdgeCanvas<BoardSize, SizeType>::Height;
      int y = y0 + edge.Dot1().Y() * canvas::EdgeCanvas<BoardSize, SizeType>::Height;
      if (edge.Dot1().X() == edge.Dot2().X()) {
        y += BaseCanvasLayer<BoardSize, SizeType>::UnitSize;
      } else {
        x += BaseCanvasLayer<BoardSize, SizeType>::UnitSize;
      }
      Canvases.At(edge.Value())->move(x, y);
    }
  }

  private:
  common::Array<common::Ptr<Canvas>, model::Edge<BoardSize, SizeType>::Max, int> Canvases;
};

}  // namespace dab::frontend::layer

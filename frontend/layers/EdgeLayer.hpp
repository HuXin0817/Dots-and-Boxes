#pragma once

#include "../../src/common/Array.hpp"
#include "../../src/common/Ptr.hpp"
#include "../../src/model/Edge.hpp"
#include "../../src/model/ValueIterator.hpp"
#include "BaseCanvasLayer.hpp"

namespace dab::frontend::layer {

template <int BoardSize, typename SizeType, typename Canvas>
class EdgeLayer : public BaseCanvasLayer<BoardSize, SizeType> {
  using Base = BaseCanvasLayer<BoardSize, SizeType>;

  public:
  explicit EdgeLayer(QWidget* parent) : Base(parent) {
  }

  common::Ptr<Canvas>&
  At(const model::Edge<BoardSize, SizeType> Edge) {
    return Canvases.At(Edge.Value());
  }

  protected:
  void
  resizeEvent(QResizeEvent* event) override {
    Base::resizeEvent(event);

    const int x0 = (Base::width() - Base::BoardWidth) / 2 - Base::UnitSize;
    const int y0 = (Base::height() - Base::BoardWidth) / 2 - Base::UnitSize;

    for (const model::Edge<BoardSize, SizeType> edge :
         model::ValueIterator<model::Edge<BoardSize, SizeType>, SizeType>()) {
      int x = x0 + edge.Dot1().X() * canvas::EdgeCanvas<BoardSize, SizeType>::Height;
      int y = y0 + edge.Dot1().Y() * canvas::EdgeCanvas<BoardSize, SizeType>::Height;
      if (edge.Dot1().X() == edge.Dot2().X()) {
        y += Base::UnitSize;
      } else {
        x += Base::UnitSize;
      }
      Canvases.At(edge.Value())->move(x, y);
    }
  }

  private:
  common::Array<common::Ptr<Canvas>, model::Edge<BoardSize, SizeType>::Max, SizeType> Canvases;
};

}  // namespace dab::frontend::layer

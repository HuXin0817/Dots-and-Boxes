#pragma once

#include "../../src/common/Array.hpp"
#include "../../src/model/Edge.hpp"
#include "../../src/model/ValueIterator.hpp"
#include "BaseCanvasLayer.hpp"

template <int BoardSize, typename SizeType, typename Canvas>
class EdgeLayer : public BaseCanvasLayer<BoardSize, SizeType> {
  using Base = BaseCanvasLayer<BoardSize, SizeType>;

  public:
  explicit EdgeLayer(QWidget* parent) : Base(parent) {
  }

  std::unique_ptr<Canvas>&
  At(const Edge<BoardSize, SizeType> Edge) {
    return Canvases.At(Edge.Value());
  }

  protected:
  void
  resizeEvent(QResizeEvent* event) override {
    Base::resizeEvent(event);

    const int x0 = (Base::width() - Base::BoardWidth) / 2 - Base::UnitSize;
    const int y0 = (Base::height() - Base::BoardWidth) / 2 - Base::UnitSize;

    for (const Edge<BoardSize, SizeType> edge : ValueIterator<Edge<BoardSize, SizeType>, SizeType>()) {
      int x = x0 + edge.Dot1().X() * EdgeCanvas<BoardSize, SizeType>::Height;
      int y = y0 + edge.Dot1().Y() * EdgeCanvas<BoardSize, SizeType>::Height;
      if (edge.Dot1().X() == edge.Dot2().X()) {
        y += Base::UnitSize;
      } else {
        x += Base::UnitSize;
      }
      Canvases.At(edge.Value())->move(x, y);
    }
  }

  private:
  Array<std::unique_ptr<Canvas>, Edge<BoardSize, SizeType>::Max, SizeType> Canvases;
};

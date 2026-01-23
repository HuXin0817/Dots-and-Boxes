#pragma once

#include "../../src/common/Array.hpp"
#include "../../src/model/Edge.hpp"
#include "BaseCanvasLayer.hpp"

template <int64_t BoardSize, typename Canvas>
class EdgeLayer : public BaseCanvasLayer<BoardSize> {
  using Base = BaseCanvasLayer<BoardSize>;

  public:
  explicit EdgeLayer(QWidget* parent) : Base(parent) {
  }

  std::unique_ptr<Canvas>&
  At(const Edge<BoardSize> Edge) {
    return Canvases.At(Edge.Value());
  }

  protected:
  void
  resizeEvent(QResizeEvent* event) override {
    Base::resizeEvent(event);

    const int x0 = (Base::width() - Base::BoardWidth) / 2 - Base::UnitSize;
    const int y0 = (Base::height() - Base::BoardWidth) / 2 - Base::UnitSize;

    for (const Edge<BoardSize> edge : std::views::iota(0, Edge<BoardSize>::Max)) {
      int x = x0 + edge.Dot1().X() * EdgeCanvas<BoardSize>::Height;
      int y = y0 + edge.Dot1().Y() * EdgeCanvas<BoardSize>::Height;
      if (edge.Dot1().X() == edge.Dot2().X()) {
        y += Base::UnitSize;
      } else {
        x += Base::UnitSize;
      }
      Canvases.At(edge.Value())->move(x, y);
    }
  }

  private:
  Array<std::unique_ptr<Canvas>, Edge<BoardSize>::Max, SizeType<BoardSize>> Canvases;
};

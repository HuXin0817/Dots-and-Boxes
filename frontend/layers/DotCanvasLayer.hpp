#pragma once

#include "../../src/common/Array.hpp"
#include "../../src/model/Square.hpp"
#include "../../src/model/ValueIterator.hpp"
#include "../canvases/DotCanvas.hpp"
#include "../canvases/EdgeCanvas.hpp"
#include "BaseCanvasLayer.hpp"

template <int64_t BoardSize>
class DotCanvasLayer final : public BaseCanvasLayer<BoardSize> {
  using Base = BaseCanvasLayer<BoardSize>;

  public:
  explicit DotCanvasLayer(QWidget* parent) : Base(parent) {
    Base::resize(Base::WindowSize, Base::WindowSize);
    for (const Dot<BoardSize> dot : ValueIterator<Dot<BoardSize>, SizeType<BoardSize>>()) {
      DotCanvases.At(dot.Value()) = std::make_unique<DotCanvas<BoardSize>>(this);
    }
  }

  std::unique_ptr<DotCanvas<BoardSize>>&
  At(const Dot<BoardSize> dot) {
    return DotCanvases.At(dot.Value());
  }

  protected:
  void
  resizeEvent(QResizeEvent* event) override {
    Base::resizeEvent(event);

    const int x0 = (Base::width() - Base::BoardWidth) / 2 - Base::UnitSize;
    const int y0 = (Base::height() - Base::BoardWidth) / 2 - Base::UnitSize;

    for (int i = 0; i < Dot<BoardSize>::Size; i++) {
      for (int j = 0; j < Dot<BoardSize>::Size; j++) {
        const int x = x0 + i * EdgeCanvas<BoardSize>::Height;
        const int y = y0 + j * EdgeCanvas<BoardSize>::Height;
        DotCanvases.At(Dot<BoardSize>(i, j).Value())->move(x, y);
      }
    }
  }

  private:
  Array<std::unique_ptr<DotCanvas<BoardSize>>, Dot<BoardSize>::Max, SizeType<BoardSize>> DotCanvases;
};

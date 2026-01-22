#pragma once

#include "../../src/common/Array.hpp"
#include "../../src/model/Square.hpp"
#include "../../src/model/ValueIterator.hpp"
#include "../canvases/DotCanvas.hpp"
#include "../canvases/EdgeCanvas.hpp"
#include "BaseCanvasLayer.hpp"

template <int BoardSize, typename SizeType>
class DotCanvasLayer final : public BaseCanvasLayer<BoardSize, SizeType> {
  using Base = BaseCanvasLayer<BoardSize, SizeType>;

  public:
  explicit DotCanvasLayer(QWidget* parent) : Base(parent) {
    Base::resize(Base::WindowSize, Base::WindowSize);
    for (const Dot<BoardSize, SizeType> dot : ValueIterator<Dot<BoardSize, SizeType>, SizeType>()) {
      DotCanvases.At(dot.Value()) = std::make_unique<DotCanvas<BoardSize, SizeType>>(this);
    }
  }

  std::unique_ptr<DotCanvas<BoardSize, SizeType>>&
  At(const Dot<BoardSize, SizeType> dot) {
    return DotCanvases.At(dot.Value());
  }

  protected:
  void
  resizeEvent(QResizeEvent* event) override {
    Base::resizeEvent(event);

    const int x0 = (Base::width() - Base::BoardWidth) / 2 - Base::UnitSize;
    const int y0 = (Base::height() - Base::BoardWidth) / 2 - Base::UnitSize;

    for (int i = 0; i < Dot<BoardSize, SizeType>::Size; i++) {
      for (int j = 0; j < Dot<BoardSize, SizeType>::Size; j++) {
        const int x = x0 + i * EdgeCanvas<BoardSize, SizeType>::Height;
        const int y = y0 + j * EdgeCanvas<BoardSize, SizeType>::Height;
        DotCanvases.At(Dot<BoardSize, SizeType>(i, j).Value())->move(x, y);
      }
    }
  }

  private:
  Array<std::unique_ptr<DotCanvas<BoardSize, SizeType>>, Dot<BoardSize, SizeType>::Max, SizeType> DotCanvases;
};

#pragma once

#include "../../src/common/Array.hpp"
#include "../../src/common/Ptr.hpp"
#include "../../src/model/Square.hpp"
#include "../../src/model/ValueIterator.hpp"
#include "../canvases/DotCanvas.hpp"
#include "../canvases/EdgeCanvas.hpp"
#include "BaseCanvasLayer.hpp"

namespace dab::frontend::layer {

template <int BoardSize, typename SizeType>
class DotCanvasLayer final : public BaseCanvasLayer<BoardSize, SizeType> {
  using Base = BaseCanvasLayer<BoardSize, SizeType>;

  public:
  explicit DotCanvasLayer(QWidget* parent) : Base(parent) {
    Base::resize(Base::WindowSize, Base::WindowSize);
    for (const model::Dot<BoardSize, SizeType> dot :
         model::ValueIterator<model::Dot<BoardSize, SizeType>, SizeType>()) {
      DotCanvases.At(dot.Value()).New(this);
    }
  }

  common::Ptr<canvas::DotCanvas<BoardSize, SizeType>>&
  At(const model::Dot<BoardSize, SizeType> dot) {
    return DotCanvases.At(dot.Value());
  }

  protected:
  void
  resizeEvent(QResizeEvent* event) override {
    Base::resizeEvent(event);

    const int x0 = (Base::width() - Base::BoardWidth) / 2 - Base::UnitSize;
    const int y0 = (Base::height() - Base::BoardWidth) / 2 - Base::UnitSize;

    for (int i = 0; i < model::Dot<BoardSize, SizeType>::Size; i++) {
      for (int j = 0; j < model::Dot<BoardSize, SizeType>::Size; j++) {
        const int x = x0 + i * canvas::EdgeCanvas<BoardSize, SizeType>::Height;
        const int y = y0 + j * canvas::EdgeCanvas<BoardSize, SizeType>::Height;
        DotCanvases.At(model::Dot<BoardSize, SizeType>(i, j).Value())->move(x, y);
      }
    }
  }

  private:
  common::Array<common::Ptr<canvas::DotCanvas<BoardSize, SizeType>>, model::Dot<BoardSize, SizeType>::Max, SizeType>
      DotCanvases;
};

}  // namespace dab::frontend::layer

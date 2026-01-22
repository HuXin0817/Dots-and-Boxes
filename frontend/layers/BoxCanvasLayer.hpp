#pragma once

#include "../../src/common/Array.hpp"
#include "../../src/model/Square.hpp"
#include "../../src/model/ValueIterator.hpp"
#include "../canvases/BoxCanvas.hpp"
#include "../canvases/EdgeCanvas.hpp"
#include "BaseCanvasLayer.hpp"

template <int BoardSize, typename SizeType>
class BoxCanvasLayer final : public BaseCanvasLayer<BoardSize, SizeType> {
  using Base = BaseCanvasLayer<BoardSize, SizeType>;

  public:
  explicit BoxCanvasLayer(QWidget* parent) : Base(parent) {
    Base::resize(Base::WindowSize, Base::WindowSize);
    for (const Box<BoardSize, SizeType> box : ValueIterator<Box<BoardSize, SizeType>, SizeType>()) {
      BoxCanvases.At(box.Value()) = std::make_unique<BoxCanvas<BoardSize, SizeType>>(this);
    }
  }

  std::unique_ptr<BoxCanvas<BoardSize, SizeType>>&
  At(const Box<BoardSize, SizeType> box) {
    return BoxCanvases.At(box.Value());
  }

  protected:
  void
  resizeEvent(QResizeEvent* event) override {
    Base::resizeEvent(event);

    const int x0 = (Base::width() - Base::BoardWidth) / 2 + Base::UnitSize;
    const int y0 = (Base::height() - Base::BoardWidth) / 2 + Base::UnitSize;

    for (int i = 0; i < Box<BoardSize, SizeType>::Size; i++) {
      for (int j = 0; j < Box<BoardSize, SizeType>::Size; j++) {
        int x = x0 + i * EdgeCanvas<BoardSize, SizeType>::Height;
        int y = y0 + j * EdgeCanvas<BoardSize, SizeType>::Height;
        BoxCanvases.At(Box<BoardSize, SizeType>(i, j).Value())->move(x, y);
      }
    }
  }

  private:
  Array<std::unique_ptr<BoxCanvas<BoardSize, SizeType>>, Box<BoardSize, SizeType>::Max, SizeType> BoxCanvases;
};

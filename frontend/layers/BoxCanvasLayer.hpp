#pragma once

#include "../../src/model/Square.hpp"
#include "../canvases/BoxCanvas.hpp"
#include "../canvases/EdgeCanvas.hpp"
#include "BaseCanvasLayer.hpp"

template <int64_t BoardSize>
class BoxCanvasLayer final : public BaseCanvasLayer<BoardSize> {
  using Base = BaseCanvasLayer<BoardSize>;

  public:
  explicit BoxCanvasLayer(QWidget* parent) : Base(parent) {
    Base::resize(Base::WindowSize, Base::WindowSize);
    for (const int i : std::views::iota(0, Box<BoardSize>::Max)) {
      BoxCanvases.emplace_back(new BoxCanvas<BoardSize>(this));
    }
  }

  QPointer<BoxCanvas<BoardSize>>&
  At(const Box<BoardSize> box) {
    return BoxCanvases[box.Value()];
  }

  protected:
  void
  resizeEvent(QResizeEvent* event) override {
    Base::resizeEvent(event);

    const int x0 = (Base::width() - Base::BoardWidth) / 2 + Base::UnitSize;
    const int y0 = (Base::height() - Base::BoardWidth) / 2 + Base::UnitSize;

    for (const int i : std::views::iota(0, Box<BoardSize>::Size)) {
      for (const int j : std::views::iota(0, Box<BoardSize>::Size)) {
        const int x = x0 + i * EdgeCanvas<BoardSize>::Height;
        const int y = y0 + j * EdgeCanvas<BoardSize>::Height;
        BoxCanvases[Box<BoardSize>(i, j).Value()]->move(x, y);
      }
    }
  }

  private:
  QList<QPointer<BoxCanvas<BoardSize>>> BoxCanvases;
};

#pragma once

#include "../../src/model/Square.hpp"
#include "../canvases/DotCanvas.hpp"
#include "../canvases/EdgeCanvas.hpp"
#include "BaseCanvasLayer.hpp"

template <int64_t BoardSize>
class DotCanvasLayer final : public BaseCanvasLayer<BoardSize> {
  using Base = BaseCanvasLayer<BoardSize>;

  public:
  explicit DotCanvasLayer(QWidget* parent) : Base(parent) {
    Base::resize(Base::WindowSize, Base::WindowSize);
    for (const int i : std::views::iota(0, Dot<BoardSize>::Max)) {
      DotCanvases.emplace_back(new DotCanvas<BoardSize>(this));
    }
  }

  QPointer<DotCanvas<BoardSize>>&
  At(const Dot<BoardSize> dot) {
    return DotCanvases[dot.Value()];
  }

  protected:
  void
  resizeEvent(QResizeEvent* event) override {
    Base::resizeEvent(event);

    const int x0 = (Base::width() - Base::BoardWidth) / 2 - Base::UnitSize;
    const int y0 = (Base::height() - Base::BoardWidth) / 2 - Base::UnitSize;

    for (const int i : std::views::iota(0, Dot<BoardSize>::Size)) {
      for (const int j : std::views::iota(0, Dot<BoardSize>::Size)) {
        const int x = x0 + i * EdgeCanvas<BoardSize>::Height;
        const int y = y0 + j * EdgeCanvas<BoardSize>::Height;
        DotCanvases[Dot<BoardSize>(i, j).Value()]->move(x, y);
      }
    }
  }

  private:
  QList<QPointer<DotCanvas<BoardSize>>> DotCanvases;
};

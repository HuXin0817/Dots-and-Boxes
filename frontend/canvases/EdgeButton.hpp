#pragma once

#include "BaseEdgeCanvas.hpp"

template <int BoardSize>
class EdgeButtonCanvas final : public BaseEdgeCanvas<BoardSize> {
  public:
  explicit EdgeButtonCanvas(bool rotate, std::function<void()> callBack, QWidget* parent)
      : BaseEdgeCanvas<BoardSize>(rotate, parent), CallBack(std::move(callBack)) {
  }

  protected:
  void
  mousePressEvent(QMouseEvent* event) override {
    BaseCanvas<BoardSize>::mousePressEvent(event);

    CallBack();
  }

  private:
  std::function<void()> CallBack;
};

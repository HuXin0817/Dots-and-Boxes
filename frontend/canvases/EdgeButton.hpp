#pragma once

#include "BaseEdgeCanvas.hpp"

template <int64_t BoardSize>
class EdgeButtonCanvas final : public BaseEdgeCanvas<BoardSize> {
  using Base = BaseEdgeCanvas<BoardSize>;

  public:
  explicit EdgeButtonCanvas(const bool rotate, std::function<void()> callBack, QWidget* parent)
      : Base(rotate, parent), CallBack(std::move(callBack)) {
  }

  protected:
  void
  mousePressEvent(QMouseEvent* event) override {
    BaseCanvas<BoardSize>::mousePressEvent(event);

    CallBack();
  }

  private:
  const std::function<void()> CallBack;
};

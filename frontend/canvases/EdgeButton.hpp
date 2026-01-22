#pragma once

#include "BaseEdgeCanvas.hpp"

template <int BoardSize, typename SizeType>
class EdgeButtonCanvas final : public BaseEdgeCanvas<BoardSize, SizeType> {
  using Base = BaseEdgeCanvas<BoardSize, SizeType>;

  public:
  explicit EdgeButtonCanvas(const bool rotate, std::function<void()> callBack, QWidget* parent)
      : Base(rotate, parent), CallBack(std::move(callBack)) {
  }

  protected:
  void
  mousePressEvent(QMouseEvent* event) override {
    BaseCanvas<BoardSize, SizeType>::mousePressEvent(event);

    CallBack();
  }

  private:
  const std::function<void()> CallBack;
};

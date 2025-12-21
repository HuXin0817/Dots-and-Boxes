#pragma once

#include "BaseEdgeCanvas.hpp"

class EdgeButtonCanvas final : public BaseEdgeCanvas {
  public:
  explicit EdgeButtonCanvas(bool rotate, std::function<void()> callBack, QWidget* parent)
      : BaseEdgeCanvas(rotate, parent), CallBack(std::move(callBack)) {
  }

  protected:
  void
  mousePressEvent(QMouseEvent* event) override {
    BaseCanvas::mousePressEvent(event);

    CallBack();
  }

  private:
  std::function<void()> CallBack;
};

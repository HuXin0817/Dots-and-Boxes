#pragma once

#include "BaseEdgeCanvas.hpp"

class EdgeButtonCanvas final : public BaseEdgeCanvas {
  Q_OBJECT

  public:
  std::function<void()> CallBack;

  explicit EdgeButtonCanvas(bool rotate, std::function<void()> callBack, QWidget* parent)
      : BaseEdgeCanvas(rotate, parent), CallBack(std::move(callBack)) {
  }

  protected:
  void
  mousePressEvent(QMouseEvent* event) override {
    BaseCanvas::mousePressEvent(event);

    CallBack();
  }
};

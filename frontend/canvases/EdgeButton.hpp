#pragma once

#include "BaseCanvas.hpp"

class EdgeButton final : public BaseCanvas {
  Q_OBJECT

  public:
  static constexpr int A = R * 2;
  static constexpr int B = A * 5;

  std::function<void()> CallBack;
  bool Rotate = false;

  explicit EdgeButton(bool rotate, std::function<void()> callBack, QWidget* parent)
      : BaseCanvas(parent), CallBack(std::move(callBack)), Rotate(rotate) {
    if (!rotate) {
      resize(QSize(A, B));
    } else {
      resize(QSize(B, A));
    }
  }

  protected:
  void
  mousePressEvent(QMouseEvent* event) override {
    BaseCanvas::mousePressEvent(event);

    CallBack();
  }
};

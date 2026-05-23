#pragma once

#include <functional>

#include "BaseCanvas.h"

namespace dab::__detail__::frontend {

class EdgeCanvas : public BaseCanvas {
  Q_OBJECT
  Q_DISABLE_COPY(EdgeCanvas)

 public:
  static int Width() { return UnitSize * 2; }
  static int Height() { return Width() * 5; }

  EdgeCanvas(Edge edge, std::function<void(Edge)> callback, QWidget* parent);

 public Q_SLOTS:
  void Resize() { setFixedSize(Value.Rotate() ? Width() : Height(), Value.Rotate() ? Height() : Width()); }

 protected:
  void mousePressEvent(QMouseEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

 private:
  Edge Value;
  std::function<void(Edge)> Callback;

  QColor Color() const;
};

}  // namespace dab::__detail__::frontend
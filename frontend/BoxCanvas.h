#pragma once

#include "BaseCanvas.h"
#include "EdgeCanvas.h"

namespace dab::__detail__::frontend {

class BoxCanvas : public BaseCanvas {
  Q_OBJECT
  Q_DISABLE_COPY(BoxCanvas)

 public:
  static int Width() { return EdgeCanvas::Height() - 2 * UnitSize; }

  BoxCanvas(Box box, QWidget* parent) : BaseCanvas(parent), Value(box) { Resize(); }

 public Q_SLOTS:
  void Resize() { setFixedSize(Width(), Width()); }

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  Box Value;

  QColor Color() const;
};

}  // namespace dab::__detail__::frontend